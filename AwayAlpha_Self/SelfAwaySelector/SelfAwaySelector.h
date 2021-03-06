//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Feb  8 11:34:13 2015 by ROOT version 5.34/21
// from TTree DSTtree/tree of selected events
// found on file: DST_Run011083.root
//////////////////////////////////////////////////////////

#ifndef SelfAwaySelector_h
#define SelfAwaySelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TNtuple.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.
using namespace std;

class SelfAwaySelector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           od_eventID;
   Double_t        od_gps_fine;
   Double_t        od_gps_coarse;
   Double_t        od_timestamp;
   Int_t           od_nclusters;
   Double_t        od_wt_charge;
   vector<double>  *od_cluster_charge;
   vector<double>  *od_cluster_start;
   vector<double>  *od_cluster_height;
   vector<double>  *od_cluster_multiplicity;
   vector<int>     *od_cluster_pass_multcut;
   vector<double>  *od_cluster_dtprompt;

   // List of branches
   TBranch        *b_od_eventID;   //!
   TBranch        *b_od_gps_fine;   //!
   TBranch        *b_od_gps_coarse;   //!
   TBranch        *b_od_timestamp;   //!
   TBranch        *b_od_nclusters;   //!
   TBranch        *b_od_wt_charge;   //!
   TBranch        *b_od_cluster_charge;   //!
   TBranch        *b_od_cluster_start;   //!
   TBranch        *b_od_cluster_height;   //!
   TBranch        *b_od_cluster_multiplicity;   //!
   TBranch        *b_od_cluster_pass_multcut;   //!
   TBranch        *b_od_cluster_dtprompt;   //!

 SelfAwaySelector(TTree * /*tree*/ =0) : fChain(0),
    nv_all_gps_hist(0),
    nv_all_ene_hist(0),
    nv_all_small_ene_hist(0),
    nv_all_gps_ene_hist(0),
    nv_all_ntuple(0),
   
    nv_all_charge_height_multiplicity_hist(0),
    nv_fail_gps_hist(0),
    nv_fail_ene_hist(0),
    nv_fail_small_ene_hist(0),
    nv_fail_gps_ene_hist(0),
    nv_fail_ntuple(0),

    nv_gps_hist(0),
    nv_ene_hist(0),
    nv_small_ene_hist(0),
    nv_gps_ene_hist(0),
    nv_ntuple(0),
    nv_charge_height_multiplicity_hist(0),
    nv_gamma_full_ene_hist(0),
    nv_gamma_full_small_ene_hist(0),

    nv_gamma_coincidence_gps_hist(0),
    nv_gamma_coincidence_ene_hist(0),
    nv_gamma_coincidence_small_ene_hist(0),
    nv_gamma_coincidence_gps_ene_hist(0),
    nv_gamma_coincidence_ntuple(0),
   
    nv_gamma_prompt_gps_hist(0),
    nv_gamma_prompt_ene_hist(0),
    nv_gamma_prompt_small_ene_hist(0),
    nv_gamma_prompt_gps_ene_hist(0),
    nv_gamma_prompt_ntuple(0),
   
    nv_gamma_delay_gps_hist(0),
    nv_gamma_delay_ene_hist(0),
    nv_gamma_delay_small_ene_hist(0),
    nv_gamma_delay_gps_ene_hist(0),
    nv_gamma_delay_ntuple(0),
   
    nv_gamma_after_gps_hist(0),
    nv_gamma_after_ene_hist(0),
    nv_gamma_after_small_ene_hist(0),
    nv_gamma_after_gps_ene_hist(0),
    nv_gamma_after_ntuple(0),
   
    nv_gamma_first_gps_hist(0),
    nv_gamma_first_ene_hist(0),
    nv_gamma_first_small_ene_hist(0),
    nv_gamma_first_gps_ene_hist(0),
    nv_gamma_first_ntuple(0),
   
    nv_gamma_late_gps_hist(0),
    nv_gamma_late_ene_hist(0),
    nv_gamma_late_small_ene_hist(0),
    nv_gamma_late_gps_ene_hist(0),
     nv_gamma_late_ntuple(0),
     id(0)
     { }
   virtual ~SelfAwaySelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

 private:
   //Create Histograms and Fill them                                                                                                 
   void BookHistograms();
   void FillHistograms();
   TH1F* nv_all_gps_hist;
   TH1F* nv_all_ene_hist;
   TH1F* nv_all_small_ene_hist;
   TH2F* nv_all_gps_ene_hist;
   TNtuple* nv_all_ntuple;
   
   TH2F* nv_all_charge_height_multiplicity_hist;
   TH1F* nv_fail_gps_hist;
   TH1F* nv_fail_ene_hist;
   TH1F* nv_fail_small_ene_hist;
   TH2F* nv_fail_gps_ene_hist;
   TNtuple* nv_fail_ntuple;

   TH1F* nv_gps_hist;
   TH1F* nv_ene_hist;
   TH1F* nv_small_ene_hist;
   TH2F* nv_gps_ene_hist;
   TNtuple* nv_ntuple ;
   TH2F* nv_charge_height_multiplicity_hist;
   TH1F* nv_gamma_full_ene_hist;
   TH1F* nv_gamma_full_small_ene_hist;

   TH1F* nv_gamma_coincidence_gps_hist;
   TH1F* nv_gamma_coincidence_ene_hist;
   TH1F* nv_gamma_coincidence_small_ene_hist;
   TH2F* nv_gamma_coincidence_gps_ene_hist;
   TNtuple* nv_gamma_coincidence_ntuple;
   
   TH1F* nv_gamma_prompt_gps_hist;
   TH1F* nv_gamma_prompt_ene_hist;
   TH1F* nv_gamma_prompt_small_ene_hist;
   TH2F* nv_gamma_prompt_gps_ene_hist;
   TNtuple* nv_gamma_prompt_ntuple;
   
   TH1F* nv_gamma_delay_gps_hist;
   TH1F* nv_gamma_delay_ene_hist;
   TH1F* nv_gamma_delay_small_ene_hist;
   TH2F* nv_gamma_delay_gps_ene_hist;
   TNtuple* nv_gamma_delay_ntuple;
   
   TH1F* nv_gamma_after_gps_hist;
   TH1F* nv_gamma_after_ene_hist;
   TH1F* nv_gamma_after_small_ene_hist;
   TH2F* nv_gamma_after_gps_ene_hist;
   TNtuple* nv_gamma_after_ntuple;
   
   TH1F* nv_gamma_first_gps_hist;
   TH1F* nv_gamma_first_ene_hist;
   TH1F* nv_gamma_first_small_ene_hist;
   TH2F* nv_gamma_first_gps_ene_hist;
   TNtuple* nv_gamma_first_ntuple;
   
   TH1F* nv_gamma_late_gps_hist;
   TH1F* nv_gamma_late_ene_hist;
   TH1F* nv_gamma_late_small_ene_hist;
   TH2F* nv_gamma_late_gps_ene_hist;
   TNtuple* nv_gamma_late_ntuple;

   TH1F *id;
   
   ClassDef(SelfAwaySelector,0);
};

#endif

#ifdef SelfAwaySelector_cxx
void SelfAwaySelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   od_cluster_charge = 0;
   od_cluster_start = 0;
   od_cluster_height = 0;
   od_cluster_multiplicity = 0;
   od_cluster_pass_multcut = 0;
   od_cluster_dtprompt = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("od_eventID", &od_eventID, &b_od_eventID);
   fChain->SetBranchAddress("od_gps_fine", &od_gps_fine, &b_od_gps_fine);
   fChain->SetBranchAddress("od_gps_coarse", &od_gps_coarse, &b_od_gps_coarse);
   fChain->SetBranchAddress("od_timestamp", &od_timestamp, &b_od_timestamp);
   fChain->SetBranchAddress("od_nclusters", &od_nclusters, &b_od_nclusters);
   fChain->SetBranchAddress("od_wt_charge", &od_wt_charge, &b_od_wt_charge);
   fChain->SetBranchAddress("od_cluster_charge", &od_cluster_charge, &b_od_cluster_charge);
   fChain->SetBranchAddress("od_cluster_start", &od_cluster_start, &b_od_cluster_start);
   fChain->SetBranchAddress("od_cluster_height", &od_cluster_height, &b_od_cluster_height);
   fChain->SetBranchAddress("od_cluster_multiplicity", &od_cluster_multiplicity, &b_od_cluster_multiplicity);
   fChain->SetBranchAddress("od_cluster_pass_multcut", &od_cluster_pass_multcut, &b_od_cluster_pass_multcut);
   fChain->SetBranchAddress("od_cluster_dtprompt", &od_cluster_dtprompt, &b_od_cluster_dtprompt);
}

Bool_t SelfAwaySelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef SelfAwaySelector_cxx
