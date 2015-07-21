#include <sstream>
#include <vector> 
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TColor.h"
using namespace std;

void Alpha_MC_diff()
{
  string Time = "Jan1AM";
  THStack *Alpha_EnergyMC_norm = new THStack("Alpha_EnergyMC_norm","Energy Spectrum;Energy [MeV]");
  THStack *Alpha_QuenchEnergyMC_norm = new THStack("Alpha_QuenchEnergyMC_norm","Quench Energy Spectrum;Energy [keVee]");

  const int N = 2;
  double ene[N] ={1,65};//keVee
  double nv_pdf_hist0_prob[N], nv_pdf_hist1_prob[N], nv_ER_pdf_hist0_prob[N],nv_ER_pdf_hist1_prob[N];
  vector<int> linecolor = Colors();
  TCanvas* c3 = new TCanvas("c3","Alpha_Energy",1200,1000);
  c3->Divide(4,3);
  TCanvas* c4 = new TCanvas("c4","Alpha_QuenchEnergy",1200,1000);
  c4->Divide(4,3);

  const int NUM = 10;
  TObjArray Alpha_EnergyMC_list(0);
  TObjArray Alpha_QuenchEnergyMC_list(0);
  TLegend *leg = new TLegend(0.45,0.25,0.95,0.7);
  TLegend *leg2 = new TLegend(0.45,0.25,0.95,0.7);
  int color = 0;	  
  for(int i=0; i<=NUM; i++)
    { 
      TFile* t = new TFile(Readdatafile(i).c_str());
      TH1F* Alpha_EnergyMC = (TH1F*) t->Get("Alpha_EnergyMC");
      TH1F* Alpha_QuenchEnergyMC = (TH1F*) t->Get("Alpha_QuenchEnergyMC");
      Alpha_EnergyMC->Rebin(50);
      Alpha_QuenchEnergyMC->Rebin(50);
      stringstream oss;
      float fraction = i; 
      if(i>5 && i<9) fraction = (i-4)*5;
      if(i==9) fraction = 50;  
      if(i==10) fraction = 0.097;  
      oss<<fraction;
      string entryname = oss.str()+"% TMB";
      leg->AddEntry(Alpha_EnergyMC,entryname.c_str(),"l");
      leg2->AddEntry(Alpha_QuenchEnergyMC,entryname.c_str(),"l"); 
      //  Calculate_Probability(nv_pdf_hist,fraction,N,ene);
      //  Calculate_Probability(nv_ER_pdf_hist,fraction,N,ene);	  
      color = linecolor.at(i);
      Alpha_EnergyMC->SetLineColor(color);
      Alpha_QuenchEnergyMC->SetLineColor(color);
      Alpha_EnergyMC->SetLineWidth(3);
      Alpha_QuenchEnergyMC->SetLineWidth(3);

      c3->cd(i+1);
      gPad->SetLogy();
      Alpha_EnergyMC->Draw();

      c4->cd(i+1);
      gPad->SetLogy();
      Alpha_QuenchEnergyMC->Draw();

      Alpha_EnergyMC_norm->Add(Alpha_EnergyMC);
      Alpha_QuenchEnergyMC_norm->Add(Alpha_QuenchEnergyMC);
      Alpha_EnergyMC_list.Add(Alpha_EnergyMC);
      Alpha_QuenchEnergyMC_list.Add(Alpha_QuenchEnergyMC);
    }

  TCanvas* c1 = new TCanvas("c1","Alpha Energy",600,400);
  c1->SetLogy();
  Alpha_EnergyMC_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c2 = new TCanvas("c2","Alpha Quench Energy",600,400);
  c2->SetLogy();
  Alpha_QuenchEnergyMC_norm->Draw("nostack");
  leg2->Draw();

  string output1="diffAlpha_tmb_"+Time+".root"; 
  TFile out1(output1.c_str(),"RECREATE");
  Alpha_EnergyMC_list.Write();
  Alpha_QuenchEnergyMC_list.Write();
  c1->Write();
  c2->Write();
  c3->Write();
  c4->Write();
  out1.Write();
  out1.Close();
}

string Readdatafile(int i)
{
  string dirname="/home/hqian/montecarlo/g4ds10/Linux-g++/Alpha/";
  string middle="Energy_Spectrum_Alpha";
  string last=".root";
  string filename;
  stringstream oss;
  if(i==0)
    filename=dirname+middle+last;
  else {
    oss<<i;
    filename=dirname+middle+"_v"+oss.str()+last;
    oss.str("");
  }
  cout<<"Processing Data file: "<<filename<<endl;
  return filename;
}

void Calculate_Probability(TH1F* nv_pdf_hist, string label, const int N, double ene[], double* prob) 
{
  for(int i=0; i<N; i++)
    {
      int bin =  nv_pdf_hist->GetXaxis()->FindBin(ene[i]);
      prob[i] = nv_pdf_hist->GetBinContent(bin);
      cout<<"For "<<label<<", Probability corresponding to Energy "<<ene[i]<<" is "<<prob[i]<<endl;
    }
}

void Calculate_Probability(TH1F* nv_pdf_hist, double fraction, const int N, double ene[]) 
{
  for(int i=0; i<N; i++)
    {
      int bin =  nv_pdf_hist->GetXaxis()->FindBin(ene[i]);
      double prob = nv_pdf_hist->GetBinContent(bin);
      cout<<"For TMB concentration "<<fraction<<", Probability corresponding to Energy "<<ene[i]<<" is "<<prob*100.<<endl;
    }
}

vector<int> Colors()
{
  vector<int> color;
  color.push_back(TColor::GetColor("#000000"));
  color.push_back(TColor::GetColor("#59FF49"));
  color.push_back(TColor::GetColor("#E8A60C"));
  color.push_back(TColor::GetColor("#FF0000"));
  color.push_back(TColor::GetColor("#5A1DE8"));
  color.push_back(TColor::GetColor("#1CFFDF"));
  color.push_back(TColor::GetColor("#B26618"));
  color.push_back(TColor::GetColor("#1D537F"));
  color.push_back(TColor::GetColor("#4A067F"));
  color.push_back(TColor::GetColor("#FF379F"));
  color.push_back(TColor::GetColor("#25DCFF"));
  color.push_back(TColor::GetColor("#92B20B"));
  color.push_back(TColor::GetColor("#79FFFF"));
  color.push_back(TColor::GetColor("#11FF8F"));
  color.push_back(TColor::GetColor("#09B23E"));
  color.push_back(TColor::GetColor("#62B21D"));
  color.push_back(TColor::GetColor("#FB78FF"));
  color.push_back(TColor::GetColor("#AE3EFF"));
  color.push_back(TColor::GetColor("#B24F18"));
  color.push_back(TColor::GetColor("#FFA762"));
  color.push_back(TColor::GetColor("#2FDAFF"));      
  
  return color;
}


  /*  THStack *nv_pdf_fine_hist_norm = new THStack("nv_pdf_fine_hist_norm","Cumulative distribution function for [-100ns,500ns] window;Energy [keVee];Probability");
  THStack *nv_ER_energy_hist_norm = new THStack("nv_ER_energy_hist_norm","Energy Spectrum for [1.5us,1ms] window;Energy [keVee]");
  THStack *nv_ER_pdf_hist_norm = new THStack("nv_ER_pdf_hist_norm","Cumulative distribution function for [1.5us,1ms] window;Energy [keVee];Probability");
  THStack *nv_ER_pdf_fine_hist_norm = new THStack("nv_ER_pdf_fine_hist_norm","Cumulative distribution function for [1.5us,1ms] window;Energy [keVee];Probability");  
  TObjArray nv_ER_pdf_list(0);
  TObjArray nv_ER_pdf_fine_list(0);
  TH1F* nv_pdf_fine_hist = (TH1F*) t->Get("nv_pdf_fine_hist");
  TH1F* nv_ER_energy_hist = (TH1F*) t->Get("nv_ER_energy_hist");
  TH1F* nv_ER_pdf_hist = (TH1F*) t->Get("nv_ER_pdf_hist");
  TH1F* nv_ER_pdf_fine_hist = (TH1F*) t->Get("nv_ER_pdf_fine_hist");        
  nv_pdf_hist->SetLineColor(color);
  nv_pdf_fine_hist->SetLineColor(color);
  nv_ER_pdf_hist->SetLineColor(color);
  nv_ER_pdf_fine_hist->SetLineColor(color);
  nv_ER_energy_hist->SetLineColor(color);      
  nv_ER_energy_hist_norm->Add(nv_ER_energy_hist);
  nv_pdf_hist_norm->Add(nv_pdf_hist);
  nv_pdf_fine_hist_norm->Add(nv_pdf_fine_hist);
  nv_ER_pdf_hist_norm->Add(nv_ER_pdf_hist);
  nv_ER_pdf_fine_hist_norm->Add(nv_ER_pdf_fine_hist);
  nv_pdf_fine_list.Add(nv_pdf_fine_hist);
  nv_ER_pdf_list.Add(nv_ER_pdf_hist);
  nv_ER_pdf_fine_list.Add(nv_ER_pdf_fine_hist);
  TCanvas* c3 = new TCanvas("c3","pdf",600,400);
  nv_pdf_fine_hist_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c4 = new TCanvas("c4","ER pdf",600,400);
  nv_ER_pdf_fine_hist_norm->Draw("nostack");
  leg2->Draw();
//  L2->Draw();

  TCanvas* c5 = new TCanvas("c5","energy",600,400);
  nv_energy_hist_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c6 = new TCanvas("c6","energy",600,400);
  nv_ER_energy_hist_norm->Draw("nostack");
  leg2->Draw();

  TCanvas* c7 = new TCanvas("c7","Total",1200,800);
  c7->Divide(2,2);
  c7->cd(1);
  gPad->SetLogy();
  nv_energy_hist_norm->Draw("nostack");
  leg->Draw();
  c7->cd(2);
  gPad->SetLogy();
  nv_ER_energy_hist_norm->Draw("nostack");
  leg2->Draw();
  c7->cd(3);
  gPad->SetLogy();
  nv_pdf_hist_norm->Draw("nostack");
  leg->Draw();
  c7->cd(4);
  gPad->SetLogy();
  nv_ER_pdf_hist_norm->Draw("nostack");
  leg2->Draw();  
  */
