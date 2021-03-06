/*
  ROOT macro to produce DSTs for alpha hunting
  *****************For Self Trigger Mode************
  *****************For source rotate away**********
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <map>
#include <ctime>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TMath.h"
#include "TNtuple.h"
#include "TAxis.h"
#include <vector>
//#include "../../darkart/Products/EventData.hh"
//#include "analysis_lib.hh"

using namespace std;
//using namespace darkart;

// Forward declaration
//void LoopOverChain(TChain* tpc_chain,TChain* od_chain ,TString outFileName = "analysis_v1.root");
void LoopOverChain(TChain* od_chain ,TString outFileName = "analysis_v1.root");
bool multiplicity_cut(Float_t height, Float_t multiplicity, Float_t charge);


void AwaySelfMacroDST(TString run_number) {

        Int_t runnum = run_number.Atoi();
	//struct VetoEvent;
        //Chain for the TPC
	//TChain* tpc_chain = new TChain("treeBuilder/Events");
	//	string  tpc_path  = "/analysis/darkart_v1_01_04/tpc/Run";
	TChain* od_chain = new TChain("odtree");
	string  od_path  = "/analysis/veto/ODRun";

	std::vector<int> run_id_list;

	run_id_list.push_back(runnum);

	TString outFileName;
	outFileName.Form("./DST_Run%06d.root",run_id_list[0]);

	//Create path and add file to chain
	TString os_tpc, os_od;
	//std::ostringstream os_tpc, os_od;
	for(vector<int>::const_iterator it = run_id_list.begin(); it != run_id_list.end(); it++){
	  //	  os_tpc.Form("%06d/Run%06d.root",*it,*it);
	  //  os_tpc.Prepend(tpc_path);	  
	  
	  os_od.Form("%06d/ODRun%06d.root",*it,*it);
	  os_od.Prepend(od_path);
	
	  cout << "Adding file in OD chain: " << os_od << '\n';
	  //  cout << "Adding file in TPC chain: " << os_tpc << endl;

	  //	  tpc_chain  ->Add (os_tpc);		 
	  od_chain   ->Add (os_od); 
	}

	//       	LoopOverChain(tpc_chain, od_chain,outFileName);
       	LoopOverChain(od_chain,outFileName);
}


//void LoopOverChain(TChain* tpc_chain, TChain* od_chain, TString outFileName){
void LoopOverChain(TChain* od_chain, TString outFileName){
  
	const Double_t od_coinc_wind = 10150.; //[ns] obtained from validation fits
	//Add two more variables for the self trigger modes match
	const Double_t od_delay_window = 1000;  //[us]
	const Double_t od_prompt_window = -100.; //[us]
	//OD TChain
	Int_t    od_run                           = -1;
	Int_t    od_event                         = -1;
	Int_t    od_lsv_nclusters                 = -1;
	Float_t  od_lsv_cluster_charge[200]       = {}; //[PE]
	Float_t  od_lsv_cluster_start[200]        = {}; //[ns]
	Float_t  od_lsv_cluster_height[200]       = {}; 
	Float_t  od_lsv_cluster_multiplicity[200] = {};
	UInt_t   od_gps_fine_time_counter         = 0;  //clock cycles
	UShort_t od_pps_counter                   = 0;  //[s]
	Float_t  od_wt_total_charge               = -1.;//[PE]
	Int_t    od_bad_time_alignment            = -1; //1 if true
	
	od_chain->SetBranchStatus("*",0);
	od_chain->SetBranchStatus("run",1);
	od_chain->SetBranchStatus("event_number",1);
	od_chain->SetBranchStatus("pps_counter",1);
	od_chain->SetBranchStatus("gps_fine_time_counter",1);
	od_chain->SetBranchStatus("lsv_n_clusters",1);
	od_chain->SetBranchStatus("lsv_cluster_fixed_width_charge",1);
	od_chain->SetBranchStatus("lsv_cluster_start_ns",1);
	od_chain->SetBranchStatus("lsv_cluster_max_multiplicity",1);
	od_chain->SetBranchStatus("lsv_cluster_height",1);
	od_chain->SetBranchStatus("wt_total_spe_charge",1);
	od_chain->SetBranchStatus("bad_time_alignment",1);
	
	od_chain->SetBranchAddress("run", &od_run);
	od_chain->SetBranchAddress("event_number", &od_event);
	od_chain->SetBranchAddress("gps_fine_time_counter", &od_gps_fine_time_counter);
	od_chain->SetBranchAddress("pps_counter", &od_pps_counter);
	od_chain->SetBranchAddress("lsv_n_clusters", &od_lsv_nclusters);
	od_chain->SetBranchAddress("lsv_cluster_fixed_width_charge", &od_lsv_cluster_charge);
	od_chain->SetBranchAddress("lsv_cluster_start_ns", &od_lsv_cluster_start);
	od_chain->SetBranchAddress("lsv_cluster_height", &od_lsv_cluster_height);
	od_chain->SetBranchAddress("lsv_cluster_max_multiplicity", &od_lsv_cluster_multiplicity);
      	od_chain->SetBranchAddress("wt_total_spe_charge", &od_wt_total_charge);
	od_chain->SetBranchAddress("bad_time_alignment", &od_bad_time_alignment);

	std::cout << "Saving output to "<<outFileName.Data()<<std::endl;
	TFile* f = new TFile(outFileName.Data(), "RECREATE");
	//Create DSTtree
	TTree * DSTtree = new TTree ("DSTtree", "tree of selected events");
	
	Int_t od_eventID       = -1;                                                                                         
        Int_t od_nclusters     = -1;                                                                                                 
        Double_t od_gps_fine   = -1.;                                                                                                
        Double_t od_gps_coarse = -1.;                                                                                                
        Double_t od_timestamp  = -1.;                                                                                                
        Double_t od_wt_charge  = -1.;                                                                                                
        std::vector<Double_t> od_cluster_charge;                                                                                     
        std::vector<Double_t> od_cluster_start;                                                                                      
        std::vector<Double_t> od_cluster_height;                                                                                     
        std::vector<Double_t> od_cluster_multiplicity;                                                                              
        std::vector<Int_t> od_cluster_pass_multcut;                                                                                
        std::vector<Double_t> od_cluster_dtprompt;                                                                                  
                                                                                                                                  
        DSTtree->Branch("od_eventID",&od_eventID,"od_eventID/I");                                                                    
        DSTtree->Branch("od_gps_fine",&od_gps_fine,"od_gps_fine/D");                                                                 
        DSTtree->Branch("od_gps_coarse",&od_gps_coarse,"od_gps_coarse/D");                                                           
        DSTtree->Branch("od_timestamp", &od_timestamp,"od_timestamp/D"); //time in ns                                                
        DSTtree->Branch("od_nclusters", &od_nclusters,"od_nclusters/I");                                                             
        DSTtree->Branch("od_wt_charge", &od_wt_charge,"od_wt_charge/D");                                                             
        DSTtree->Branch("od_cluster_charge", &od_cluster_charge);                                                                    
        DSTtree->Branch("od_cluster_start", &od_cluster_start); // time in ns                                                        
        DSTtree->Branch("od_cluster_height", &od_cluster_height);                                                                    
        DSTtree->Branch("od_cluster_multiplicity", &od_cluster_multiplicity);                                                        
        DSTtree->Branch("od_cluster_pass_multcut", &od_cluster_pass_multcut);                                                        
        DSTtree->Branch("od_cluster_dtprompt", &od_cluster_dtprompt);                                                              

	//////////////////////////////////////////////////////////////////////////
	/////////////////     BEGIN LOOP OVER EVENTS       ///////////////////////
	//////////////////////////////////////////////////////////////////////////
	Int_t last_od_event=0;
	const Int_t n_od_begin = (last_od_event - 1000 <= 0 ) ? 0 : (last_od_event - 1000); 
	const Int_t n_od_end = od_chain->GetEntries();
	
	//	int od_counts = -1;
	for (Int_t n_od = n_od_begin; n_od < n_od_end; n_od++){
	  od_chain->GetEntry(n_od);
	  if (od_pps_counter==0)   continue;

	  //cout << "loop on od events: " <<  n_od_begin << '\t' <<  n_od_end << '\n'; 			             
	  if (od_bad_time_alignment>0){
	    cout << "failing od_time alignment" << '\n';
	    continue;
	  }
	  Double_t delay_veto = 0.;//5.;//54.*20.e-3;  [us]
	  Double_t tmp_od_timestamp     = od_pps_counter*1.e+6 + od_gps_fine_time_counter*20.e-3; // [us]
	  last_od_event=n_od;
	  //  ++od_counts;
	  od_eventID = od_event;
	  od_nclusters = od_lsv_nclusters;
	  od_gps_fine = od_gps_fine_time_counter+0.;
	  od_gps_coarse = od_pps_counter+0.;
	  od_wt_charge = od_wt_total_charge;
	  od_timestamp = tmp_od_timestamp;
	  
	  if(od_cluster_charge.size()){
	  od_cluster_charge.clear();
	  od_cluster_start.clear();
	  od_cluster_height.clear();
	  od_cluster_multiplicity.clear();
	  od_cluster_pass_multcut.clear();
	  od_cluster_dtprompt.clear();
	  }

	  for (Int_t n_clu = 0 ; n_clu < od_lsv_nclusters; n_clu++){
	    od_cluster_charge.push_back(od_lsv_cluster_charge[n_clu]);
	    od_cluster_start.push_back(od_lsv_cluster_start[n_clu]*1.e-3);
	    od_cluster_height.push_back(od_lsv_cluster_height[n_clu]);
	    od_cluster_multiplicity.push_back(od_lsv_cluster_multiplicity[n_clu]);
	      
	    if (multiplicity_cut(od_lsv_cluster_height[n_clu], od_lsv_cluster_multiplicity[n_clu], od_lsv_cluster_charge[n_clu] ))
	      od_cluster_pass_multcut.push_back(1);
	    else od_cluster_pass_multcut.push_back(0);
	    
	    od_cluster_dtprompt.push_back((od_lsv_cluster_start[n_clu] - od_coinc_wind)*1.e-3);
	  } // end loop on clusters 			    	    
	  DSTtree                     ->Fill();
	} // end on selected od events
	
	/////////////////////////////////////////////////////////////////////////
	/////////////////     WRITE HISTOGRAMS TO FILE       ////////////////////
	/////////////////////////////////////////////////////////////////////////

	f->cd();
//	DSTtree->Write();
//	total_s1_f90_hist->Write();
//	total_s1_hist->Write();  
	f->Write();
	f->Close();
    
}

bool multiplicity_cut(Float_t height, Float_t multiplicity, Float_t charge){
	return height/multiplicity < (2.563e7 + TMath::Sqrt(1.574e14+1.390e12*(charge-14.40)*(charge-14.40)));
}

#ifndef __CINT__
int main(int argc, char **argv) {
  if ( argc == 2 ) {
    std::cout << "\n==========> analysis <=============" << std::endl;
    AwaySelfMacroDST(argv[1]);
  } else {
    std::cout << "Usage:" <<argc<< std::endl;
    std::cout << "./SelfMacroDST run_number" << std::endl;
  }

  std::cout << "==> Application finished." << std::endl;
  return 0;
}
#endif /* __CINT __ */



/*
struct VetoEvent{
  VetoEvent(): od_eventID(-1), od_nclusters(-1), od_gps_fine(-1),
	       od_gps_coarse(-1), od_timestamp(-1), od_wt_charge(-1)
	       //,od_counts(0)
  {}
  //  Int_t od_counts;
  Int_t od_eventID;
  Int_t od_nclusters;
  Double_t od_gps_fine;
  Double_t od_gps_coarse;
  Double_t od_timestamp; //[us]
  //  Double_t od_tpc_timestamp_diff; //[us] timestamp difference between tpc and veto. od_timestamp - tpc_timestamp
  Double_t od_wt_charge;
  std::vector<Double_t> od_cluster_charge; //[PE]
  std::vector<Double_t> od_cluster_start; //[us]
  std::vector<Double_t> od_cluster_height; 
  std::vector<Double_t> od_cluster_multiplicity;
  std::vector<Int_t> od_cluster_pass_multcut;
  std::vector<Double_t> od_cluster_dtprompt; //[us]
}; 
*/

//typedef std::vector<VetoEvent> VetoEventVec;
  /*	//Const for TPC events
        const Double_t t_drift_min        = 10.; //[us]
	const Double_t t_drift_max        = 373.3; //[us]
	const Double_t t_drift_delta      = 10; //[us]
	const Double_t t_s3_sep_min       = 372.; //372.; //[us]
	const Double_t t_s3_sep_max       = 400.; //390.; //[us]
	const Double_t electron_lifetime  = 4733; //3338; //[us]
	const Int_t N_CHANNELS            = 38;

	//Variable limits for cuts
	const Double_t tpc_s1_start_time_min = -6.10, tpc_s1_start_time_max = -6.0;
	const Double_t tpc_s1_late_min = 3500, tpc_s1_late_max = 35000;
	const Double_t tpc_s1_min = 60, tpc_s1_max = 1000;
	const Double_t tpc_total_f90Nmin = 0.6, tpc_total_f90Nmax = 0.85;
	const Double_t tpc_total_f90Gmin = 0.1, tpc_total_f90Gmax = 0.4;
  */

	/*    Int_t tpc_events = tpc_chain->GetEntries();
        if (tpc_events == 0){
	    std::cout<<"No events loaded in TChain. Aborting..."<<std::endl;
	    return;
	}
	
	cout << "Total number of events: "<<tpc_events << endl;
	
	//TPC TChain
        EventData* event = NULL;
	tpc_chain->SetBranchAddress("EventData", &event);  
	*/
	/*	TH1F* s1_startime_hist            = new TH1F("s1_startime_hist", "Drift Time", 5000, -10., 10.);
	TH1F* total_s1_hist               = new TH1F("total_s1_hist", "S1 Spectrum", 10000, 0, 10000);
	TH1F* total_f90_hist              = new TH1F("total_f90_hist", "F90 Distribution", 110, 0, 1.3);
	TH2F* total_s1_f90_hist           = new TH2F("total_s1_f90_hist", "F90 vs S1; S1 [p.e.]; F90", 10000, 0, 10000, 130, 0, 1.3);
	*/
	
	/*	Int_t    runID               = -1;
	Int_t    tpc_eventID         = -1;
	Int_t    tpc_event_type      = -1; // 0 gamma, 1 neutron, -1 else
	Double_t tpc_gps_fine        = -1.; //clock cycles
	Double_t tpc_gps_coarse      = -1.; //[s]
	Double_t tpc_s1_start_time   = -1.; //[us]
	Double_t tpc_total_s1        = -1.; //[PE]
	Double_t tpc_total_f90       = -1.; 
	Double_t tpc_t_drift         = -1.; //[us]
	Double_t tpc_s1_late         = -1.; //[PE]
	Int_t    tpc_npulses         = -1;
	Double_t tpc_timestamp       = -1;  //[us]
	Int_t    number_of_match_od_events = -1;
	
	DSTtree->Branch("runID",             &runID,              "runID/I");
	DSTtree->Branch("tpc_eventID",       &tpc_eventID,        "tpc_eventID/I");
	DSTtree->Branch("tpc_event_type",    &tpc_event_type,     "tpc_event_type/I");
	DSTtree->Branch("tpc_gps_fine",      &tpc_gps_fine,       "tpc_gps_fine/D");
	DSTtree->Branch("tpc_gps_coarse",    &tpc_gps_coarse,     "tpc_gps_coarse/D");
	DSTtree->Branch("tpc_s1_start_time", &tpc_s1_start_time,  "tpc_s1_start_time/D");
	DSTtree->Branch("tpc_total_s1",      &tpc_total_s1,       "tpc_total_s1/D");
	DSTtree->Branch("tpc_total_f90",     &tpc_total_f90,      "tpc_total_f90/D");
	DSTtree->Branch("tpc_t_drift",       &tpc_t_drift,        "tpc_t_drift/D");
	DSTtree->Branch("tpc_s1_late",       &tpc_s1_late,        "tpc_s1_late/D");
	DSTtree->Branch("tpc_npulses",       &tpc_npulses,        "tpc_npulses/I");
	DSTtree->Branch("tpc_timestamp",     &tpc_timestamp,      "tpc_timestamp/D");
	DSTtree->Branch("number_of_match_od_events",  &number_of_match_od_events, "number_of_match_od_events/I");
	*/
	//	std::vector<VetoEvent> ODEvent;
	//	DSTtree->Branch("ODEvent",&ODEvent);   //,32000,1);
//	tpc_events -= 50; // Skip last few events because end of some (very few) runs are problematic.
//	tpc_events =5000;
/*	for (Int_t n_tpc = 1; n_tpc < tpc_events ; n_tpc++){
	  Double_t DT =-100000;	

	  //Load the event
	  tpc_chain->GetEntry(n_tpc);
	  runID = event->event_info.run_id;
	  tpc_eventID = event->event_info.event_id;
	  tpc_event_type = -1;
	  
	  if(n_tpc%10000==0) cout << "Processing event: " << n_tpc << "/" << tpc_events << ", RunID: " << runID << '\n';     
	  
	  //**************Standard TPC cuts*******************
	  // Check for expected number of channels
	  
	  if ((int)event->n_channels != N_CHANNELS){
	    cout << "Event=" << event->event_info.event_id<<" has LOWER # of Channels; num ch: "<<event->n_channels<<endl;
	    continue;
	  }
	  
	  //Make sure the baseline was found on the sum channel
	  if (event->sumchannel.baseline.found_baseline == false) continue;
	  
	  //PULSE IDENTIFICATION   
	  // npulses=0, then n_phys_pulses=0, if npulses=1 then the other is one... if there is an S3, then the n_phys_pulses=2 and s1_pulse_id=0 and S2_pulse_id=1...
	  Int_t n_phys_pulses = -1, s1_pulse_id = -1, s2_pulse_id = -1;
	  ds50analysis::identify_pulses(event, n_phys_pulses, s1_pulse_id, s2_pulse_id, t_drift_max, t_drift_delta);
	  
	  //NUMBER OF PULSES
	  //Events with 0 or 1 pulses are not interesting!
	  if (n_phys_pulses==0) continue;
	  if (n_phys_pulses==1) continue;
	  
	  //CALCULATE PARAMETERS -> note: S1_pulse_id is always =0 and S2_pulse_id=1
	  tpc_total_s1 = event->pulses[s1_pulse_id].param.fixed_int1;
	  tpc_s1_start_time = event->pulses[s1_pulse_id].pulse.start_time;
	  Double_t tpc_s2_start_time = event->pulses[s2_pulse_id].pulse.start_time;
	  tpc_t_drift=-1;
	  
	  if (n_phys_pulses==2) tpc_t_drift = tpc_s2_start_time - tpc_s1_start_time;
	  
	  tpc_s1_late         = (1-event->pulses[s1_pulse_id].param.f90)*tpc_total_s1;
	  tpc_total_f90       = event->pulses[s1_pulse_id].param.f90*event->pulses[s1_pulse_id].param.npe/tpc_total_s1;
	  tpc_gps_fine        = event->event_info.gps_fine;
	  tpc_gps_coarse      = event->event_info.gps_coarse;
	  tpc_npulses         = event->n_pulses;
	  tpc_timestamp       = tpc_gps_coarse*1.e+6 + tpc_gps_fine*20.e-3; // [us]
	  
	  //*********cut analysis******************************
	  //start time cut (selecting s1 in tpc trigger position)
	  if (tpc_s1_start_time<tpc_s1_start_time_min || tpc_s1_start_time>tpc_s1_start_time_max) continue;
	  
	  //CUT to select 4.4 gammas
	  Bool_t cut_gamma   = tpc_s1_late>tpc_s1_late_min && tpc_s1_late<tpc_s1_late_max && tpc_total_f90>tpc_total_f90Gmin && tpc_total_f90<tpc_total_f90Gmax;
	  
	  //CUT to select neutrons NOTE: we could implement a cut on f90 at 2sigma of the distribution (95%)
	  Bool_t cut_neutron = tpc_total_s1>tpc_s1_min && tpc_total_s1<tpc_s1_max && tpc_total_f90>tpc_total_f90Nmin && tpc_total_f90<tpc_total_f90Nmax;
	  
	  if ( !cut_gamma && !cut_neutron ) continue;
	  
	  if(cut_gamma)tpc_event_type = 0;
	  if(cut_neutron)tpc_event_type = 1;
*/	  
	  // in slave mode 1:1 corrispondence tpc event od event (maybe just a little offset)
	  //	  if(ODEvent.size()) ODEvent.clear();

     //    cout<<endl<<"run_id: "<<runID<<" tpc_event_id: "<<tpc_eventID<<endl;
     
	//  const Int_t n_od_begin = (n_tpc - 10 < 0 ) ? 0 : (n_tpc - 10); 
	//  const Int_t n_od_end = (n_tpc + 10 > tpc_events ) ? tpc_events : (n_tpc + 10);
	// in self trigger mode there is no corrispondence tpc event od event 
	  //	    last_od_event=n_od;
	  //	    if (od_run!=runID) continue;
	   /* double od_timestamp_temp = od_pps_counter*1.e+6 + od_gps_fine_time_counter*20.e-3 - 54.*20.e-3; // [us]    
	    DT = od_timestamp_temp - tpc_timestamp;			    
	    //select only od events in coincidence with the tpc [od_prompt_window,od_delay_window]
	    if (DT<od_prompt_window ||  DT>od_delay_window) continue;*/
	   
	    //            if (tpc_gps_coarse != od_pps_counter) continue; //check coarse counter
   

//	    DT = tmp_od_timestamp - tpc_timestamp;			    
//	    DT = (od_gps_fine_time_counter - tpc_gps_fine)*20.e-3 + delay_veto;  // [us]			    
//            cout<<"DT: "<<DT<<" tmp_od_timestamp: "<<tmp_od_timestamp<<" tpc_timestamp: "<<tpc_timestamp<<endl;
	    //          if (DT > 2.e+3) break; // skip veto events if time difference is greater than 5 ms

//            cout<<"event_id:"<<od_event<<endl;
//	    if (DT<od_prompt_window ||  DT>od_delay_window) continue;
				//condition to save the events and clusters
	    //	    ODEvent.push_back(VetoEvent());
//            cout<<"matched veto!!"<<endl;
	    
	    //  ODEvent->at(od_counts).od_counts = od_counts;
	  //	    od_timestamp = od_timestamp_temp;
	  //  od_tpc_timestamp_diff = DT;
	
	  //  number_of_match_od_events = od_counts+1;
	  //  total_s1_f90_hist           ->Fill(tpc_total_s1, tpc_total_f90);
	  //	  total_s1_hist               ->Fill(tpc_total_s1);
	  
//	}//End loop over events
