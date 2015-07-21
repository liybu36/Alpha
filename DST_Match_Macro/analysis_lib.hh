/*

  Suite of functions to be used in analysis macros.

*/

#ifndef analysis_jan2014note_lib_hh
#define analysis_jan2014note_lib_hh

#include <vector>
#include <string>
#include <fstream>


#include "TFile.h"
#include "TGraph.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TMath.h"
#include "TNtuple.h"
#include "TString.h"
#include "TFile.h"

#include "../darkart/Products/EventData.hh"


using namespace std;
using namespace darkart;


namespace ds50analysis {
  
  // Geometrical correction factor for S1.
  Double_t s1_corr_factor(Double_t t_drift_max, Double_t t_drift);

  // Identify the number of physical pulses and, if possible, S1 pulse ID and S2 pulse ID.
  void identify_pulses(EventData* event,
                       Int_t & n_phys_pulses, Int_t & s1_pulse_id, Int_t & s2_pulse_id,
                       Double_t t_drift_max, Double_t t_drift_delta);


  // max_chan calculation in DarkArt uses npe and not npe_fixed
  // --> must recalculate here using fixed_int params
  void max_s1_s2(EventData* event, Int_t const s1_pulse_id, Int_t const s2_pulse_id,
                 Int_t & max_s1_chan, Double_t & max_s1,
                 Int_t & max_s2_chan, Double_t & max_s2);

  //Calculate Top/Bottom ratio for S1 and S2 pulses
  void top_bottom_ratio(EventData* event, Int_t const s1_pulse_id, Int_t const s2_pulse_id,
			Double_t & s1_top_bottom, Double_t& s2_top_bottom);

  // Cut on large max_s1_frac. Threshold is defined bin by bin.
  Bool_t large_max_s1_frac(Double_t const total_s1, Double_t const t_drift, Double_t const max_s1,
                           TFile* f);

  void get_s1_startime_cut_range(Int_t runid, Double_t & min, Double_t & max);
}








Double_t ds50analysis::s1_corr_factor(Double_t t_drift_max, Double_t t_drift)
{
  Double_t z = t_drift/(0.5*t_drift_max); // note normalization is to 0.5*t_drift_max
  // looked at Kr peaks in 15us t_drift windows (Run5330+5340), and fit these to [0]*z^5 + [1]*z^4 + [2]*z^3+[3]*z^2+[4]*z+[5].
  Double_t fit_par0 = 0.0407; 
  Double_t fit_par1 = -0.206;
  Double_t fit_par2 = 0.407;
  Double_t fit_par3 = -0.389;
  Double_t fit_par4 = 0.247;
  Double_t fit_par5 = 0.898;
  // normalizing all points on fitted curve to expected Kr peak at t_drift_max/2
  Double_t exp_Kr_peak_at_half_t_drift_max = fit_par0 + fit_par1 + fit_par2 + fit_par3 + fit_par4 + fit_par5;
  Double_t exp_Kr_peak_at_t_drift = fit_par0*z*z*z*z*z + fit_par1*z*z*z*z + fit_par2*z*z*z + fit_par3*z*z + fit_par4*z + fit_par5;
  return exp_Kr_peak_at_half_t_drift_max/exp_Kr_peak_at_t_drift; // s1 correction factor
}



void ds50analysis::identify_pulses(EventData* event,
                                   Int_t & n_phys_pulses, Int_t & s1_pulse_id, Int_t & s2_pulse_id,
                                   Double_t t_drift_min, Double_t t_drift_max)
{
  if (event->sumchannel.pulses.size() == 0)
    {
      n_phys_pulses = 0;
    }
  else if (event->sumchannel.pulses.size() == 1)
    {
      //Assume pulse is S1 ... for now
      n_phys_pulses = 1;
      s1_pulse_id = 0;
    }
  else if (event->sumchannel.pulses.size() == 2)
    {
      //Assume first pulse is S1 and second is S2 ... for now
      n_phys_pulses = 2;
      s1_pulse_id = 0;
      s2_pulse_id = 1;
    }
  else if (event->sumchannel.pulses.size() == 3)
    {
      Double_t t_drift2to3 = event->sumchannel.pulses[2].pulse.start_time - event->sumchannel.pulses[1].pulse.start_time;
/*      Double_t t_drift1to3 = event->sumchannel.pulses[2].pulse.start_time - event->sumchannel.pulses[0].pulse.start_time;
      if (t_drift1to3 > t_drift_min && t_drift1to3 <= t_drift_max){
          //Assume first pulse is S1, second is S2 and third is S3 associated to S1 ... for now
          n_phys_pulses = 2;
          s1_pulse_id = 0;
          s2_pulse_id  = 1;
      } else */
      if (t_drift2to3 > t_drift_min && t_drift2to3 <= t_drift_max) {
          //Assume first pulse is S1, second is S2 and third is S3 associated to S2 ... for now
          n_phys_pulses = 2;
          s1_pulse_id = 0;
          s2_pulse_id  = 1;
      } else {
          n_phys_pulses = event->sumchannel.pulses.size();
      }
    }
  else
    { //We don't know how many physical pulses - just set to total number of pulses for now
      n_phys_pulses = event->sumchannel.pulses.size();
    }
}

void ds50analysis::max_s1_s2(EventData* event, Int_t const s1_pulse_id, Int_t const s2_pulse_id,
                             Int_t & max_s1_chan, Double_t & max_s1,
                             Int_t & max_s2_chan, Double_t & max_s2)
{
  const Int_t nchans = event->channels.size();
  for (Int_t ch = 0; ch < nchans; ch++)
    {
      ChannelData const& channel = event->getChannelByID(ch);
      Double_t s1 = -channel.pulses[s1_pulse_id].param.fixed_int1 / channel.pmt.spe_mean;
      Double_t s2 = -channel.pulses[s2_pulse_id].param.fixed_int2 / channel.pmt.spe_mean;
      if (s1 > max_s1)
        {
          max_s1 = s1;
          max_s1_chan = ch;
        }
      if (s2 > max_s2)
        {
          max_s2 = s2;
          max_s2_chan = ch;
        }
    }
  
}

void ds50analysis::top_bottom_ratio(EventData* event, Int_t const s1_pulse_id, Int_t const s2_pulse_id,
				    Double_t & s1_top_bottom, Double_t& s2_top_bottom)
{
  const Int_t nchans = event->channels.size();
  double s1_top = 0, s1_bot = 0, s2_top = 0, s2_bot = 0;
  for (Int_t ch = 0; ch < nchans; ch++)
  {
      ChannelData const& channel = event->getChannelByID(ch);
      if (ch < 19)
      {
	  s1_bot += -channel.pulses[s1_pulse_id].param.fixed_int1 / channel.pmt.spe_mean;
	  s2_bot += -channel.pulses[s2_pulse_id].param.fixed_int2 / channel.pmt.spe_mean;
      }
      else
      {
	  s1_top += -channel.pulses[s1_pulse_id].param.fixed_int1 / channel.pmt.spe_mean;
	  s2_top += -channel.pulses[s2_pulse_id].param.fixed_int2 / channel.pmt.spe_mean;
      }
      
  }
  s1_top_bottom = s1_top/s1_bot;
  s2_top_bottom = s2_top/s2_bot;
}

Bool_t ds50analysis::large_max_s1_frac(Double_t const total_s1, Double_t const t_drift, Double_t const max_s1, TFile* f)
{
  const int n_normal_tdrift_slices = 35;
  const int normal_tdrift_slice_width = 10;
  const int n_small_tdrift_slices = 5; // # of smaller tdrift slices near the bottom of the TPC
  const int small_tdrift_slice_width = 5;
  const int n_total_tdrift_slices = n_normal_tdrift_slices + n_small_tdrift_slices;
  
  // First find the t_drift slice that this event belongs to.
  Int_t tdrift_slice = -1;
  for (Int_t i=0; i<n_total_tdrift_slices; i++) {
    if ( i < n_normal_tdrift_slices && t_drift > i*normal_tdrift_slice_width && t_drift <= (i+1)*normal_tdrift_slice_width) {
      tdrift_slice = i;
      break;
    }
    else if ( i >= n_normal_tdrift_slices &&
              t_drift >  n_normal_tdrift_slices*normal_tdrift_slice_width + (i  -n_normal_tdrift_slices)*small_tdrift_slice_width &&
              t_drift <= n_normal_tdrift_slices*normal_tdrift_slice_width + (i+1-n_normal_tdrift_slices)*small_tdrift_slice_width ) {
      tdrift_slice = i;
      break;
    }
  }
  if (t_drift > n_normal_tdrift_slices*normal_tdrift_slice_width + n_small_tdrift_slices*small_tdrift_slice_width)
    tdrift_slice = n_total_tdrift_slices - 1;
  if (t_drift == 0)
    tdrift_slice = 0;

  // Check that we've found one of the expected tdrift slices.
  if (tdrift_slice < 0 || tdrift_slice >= n_total_tdrift_slices) {
    std::cout << "WARNING (large_max_s1_frac): could not find tdrift slice." <<std::endl;
    return true;
  }

  // Extract the TGraph with the s1-dependent acceptance threshold for this tdrift slice.
  f->cd();
  std::ostringstream ssTGraph;
  ssTGraph << "max_s1_frac_thresh_tdrift_slice_"<<tdrift_slice;
  TGraph* thresholds;
  f->GetObject(ssTGraph.str().c_str(), thresholds);

  // Evaluate the TGraph to get the threshold for this total_s1.
  double threshold = thresholds->Eval(total_s1);

  // Decide whether this event passes.
  bool result = true;
  if (max_s1 / total_s1 < threshold)
    result = false;

  return result;
}


void ds50analysis::get_s1_startime_cut_range(Int_t runid, Double_t & min, Double_t & max){
  if(runid<7344){ // 7344 is first G2 trigger except test runs. see Elog711 & 719
     //Setting for normal runs without g2 trigger
     min = -0.25;
     max = -0.15;
  } else if(runid<7641){// before Run7393 setting was threshold 380, gate 3 us, prescale 33 // then change to threshold 320 until Run7641
     //Setting for g2 trigger integration window 3 us
     min = -4.10;
     max = -4.00;

  }else{// after Run7641 setting was changed to threshold 360, gate 5 us, prescale 33
     //Setting for g2 trigger integration window 5 us
     min = -6.10;
     max = -6.00;
  }
}

  
Bool_t AddFile2Chain(TString Inputfilelist, TChain &chain){
  Bool_t IsChained(false);
  ifstream inputStream(Inputfilelist.Data());
  if (!inputStream.is_open()) {
    cout << "can not open list file"<< endl;
    return false;
  }
  cout<<"Open file list: "<<Inputfilelist.Data()<<endl;

  char line[512];
  for (; inputStream.good();) {
    inputStream.getline(line, 512);
    if (!inputStream.good()) continue;

    TFile *ftmp = new TFile(line);
    //----------
    if (!(ftmp->IsOpen())){
      cout << line << " open failed ! not chained" << endl;
      continue;
    }
    if (ftmp->IsZombie()) {
      cout << "sth. very wrong with " << line << ", not chained " << endl;
      continue;
    }
    if (ftmp->TestBit(1024)) {
      cout << "revocer procedure applied to " << line << endl;
      continue;
    }
    //--------------------------
    if (ftmp && ftmp->IsOpen() && ftmp->GetNkeys()) {
      cout << "add file " << line << endl;
      chain.Add(line);
      IsChained=true;
    } else {
      cout << " cannot open file " << line << endl;
    }
    delete ftmp;
  }
  return IsChained;
}
#endif
