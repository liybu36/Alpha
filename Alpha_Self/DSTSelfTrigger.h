//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb  5 13:14:22 2015 by ROOT version 5.34/21
// from TTree DSTtree/tree of selected events
// found on file: DST_Run011010.root
//////////////////////////////////////////////////////////

#ifndef DSTSelfTrigger_h
#define DSTSelfTrigger_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.
   const Int_t kMaxODEvent = 3;

class DSTSelfTrigger : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           runID;
   Int_t           tpc_eventID;
   Int_t           tpc_event_type;
   Double_t        tpc_gps_fine;
   Double_t        tpc_gps_coarse;
   Double_t        tpc_s1_start_time;
   Double_t        tpc_total_s1;
   Double_t        tpc_total_f90;
   Double_t        tpc_t_drift;
   Double_t        tpc_s1_late;
   Int_t           tpc_npulses;
   Double_t        tpc_timestamp;
   Int_t           number_of_match_od_events;
   Int_t           ODEvent_;
   Int_t           ODEvent_od_eventID[kMaxODEvent];   //[ODEvent_]
   Int_t           ODEvent_od_nclusters[kMaxODEvent];   //[ODEvent_]
   Double_t        ODEvent_od_gps_fine[kMaxODEvent];   //[ODEvent_]
   Double_t        ODEvent_od_gps_coarse[kMaxODEvent];   //[ODEvent_]
   Double_t        ODEvent_od_timestamp[kMaxODEvent];   //[ODEvent_]
   Double_t        ODEvent_od_tpc_timestamp_diff[kMaxODEvent];   //[ODEvent_]
   Double_t        ODEvent_od_wt_charge[kMaxODEvent];   //[ODEvent_]
   vector<Double_t> ODEvent_od_cluster_charge[kMaxODEvent];
   vector<Double_t> ODEvent_od_cluster_start[kMaxODEvent];
   vector<Double_t> ODEvent_od_cluster_height[kMaxODEvent];
   vector<Double_t> ODEvent_od_cluster_multiplicity[kMaxODEvent];
   vector<Int_t>   ODEvent_od_cluster_pass_multcut[kMaxODEvent];
   vector<Double_t> ODEvent_od_cluster_dtprompt[kMaxODEvent];

   // List of branches
   TBranch        *b_runID;   //!
   TBranch        *b_tpc_eventID;   //!
   TBranch        *b_tpc_event_type;   //!
   TBranch        *b_tpc_gps_fine;   //!
   TBranch        *b_tpc_gps_coarse;   //!
   TBranch        *b_tpc_s1_start_time;   //!
   TBranch        *b_tpc_total_s1;   //!
   TBranch        *b_tpc_total_f90;   //!
   TBranch        *b_tpc_t_drift;   //!
   TBranch        *b_tpc_s1_late;   //!
   TBranch        *b_tpc_npulses;   //!
   TBranch        *b_tpc_timestamp;   //!
   TBranch        *b_number_of_match_od_events;   //!
   TBranch        *b_ODEvent_;   //!
   TBranch        *b_ODEvent_od_eventID;   //!
   TBranch        *b_ODEvent_od_nclusters;   //!
   TBranch        *b_ODEvent_od_gps_fine;   //!
   TBranch        *b_ODEvent_od_gps_coarse;   //!
   TBranch        *b_ODEvent_od_timestamp;   //!
   TBranch        *b_ODEvent_od_tpc_timestamp_diff;   //!
   TBranch        *b_ODEvent_od_wt_charge;   //!
   TBranch        *b_ODEvent_od_cluster_charge;   //!
   TBranch        *b_ODEvent_od_cluster_start;   //!
   TBranch        *b_ODEvent_od_cluster_height;   //!
   TBranch        *b_ODEvent_od_cluster_multiplicity;   //!
   TBranch        *b_ODEvent_od_cluster_pass_multcut;   //!
   TBranch        *b_ODEvent_od_cluster_dtprompt;   //!

   DSTSelfTrigger(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~DSTSelfTrigger() { }
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

   ClassDef(DSTSelfTrigger,0);
};

#endif

#ifdef DSTSelfTrigger_cxx
void DSTSelfTrigger::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runID", &runID, &b_runID);
   fChain->SetBranchAddress("tpc_eventID", &tpc_eventID, &b_tpc_eventID);
   fChain->SetBranchAddress("tpc_event_type", &tpc_event_type, &b_tpc_event_type);
   fChain->SetBranchAddress("tpc_gps_fine", &tpc_gps_fine, &b_tpc_gps_fine);
   fChain->SetBranchAddress("tpc_gps_coarse", &tpc_gps_coarse, &b_tpc_gps_coarse);
   fChain->SetBranchAddress("tpc_s1_start_time", &tpc_s1_start_time, &b_tpc_s1_start_time);
   fChain->SetBranchAddress("tpc_total_s1", &tpc_total_s1, &b_tpc_total_s1);
   fChain->SetBranchAddress("tpc_total_f90", &tpc_total_f90, &b_tpc_total_f90);
   fChain->SetBranchAddress("tpc_t_drift", &tpc_t_drift, &b_tpc_t_drift);
   fChain->SetBranchAddress("tpc_s1_late", &tpc_s1_late, &b_tpc_s1_late);
   fChain->SetBranchAddress("tpc_npulses", &tpc_npulses, &b_tpc_npulses);
   fChain->SetBranchAddress("tpc_timestamp", &tpc_timestamp, &b_tpc_timestamp);
   fChain->SetBranchAddress("number_of_match_od_events", &number_of_match_od_events, &b_number_of_match_od_events);
   fChain->SetBranchAddress("ODEvent", &ODEvent_, &b_ODEvent_);
   fChain->SetBranchAddress("ODEvent.od_eventID", ODEvent_od_eventID, &b_ODEvent_od_eventID);
   fChain->SetBranchAddress("ODEvent.od_nclusters", ODEvent_od_nclusters, &b_ODEvent_od_nclusters);
   fChain->SetBranchAddress("ODEvent.od_gps_fine", ODEvent_od_gps_fine, &b_ODEvent_od_gps_fine);
   fChain->SetBranchAddress("ODEvent.od_gps_coarse", ODEvent_od_gps_coarse, &b_ODEvent_od_gps_coarse);
   fChain->SetBranchAddress("ODEvent.od_timestamp", ODEvent_od_timestamp, &b_ODEvent_od_timestamp);
   fChain->SetBranchAddress("ODEvent.od_tpc_timestamp_diff", ODEvent_od_tpc_timestamp_diff, &b_ODEvent_od_tpc_timestamp_diff);
   fChain->SetBranchAddress("ODEvent.od_wt_charge", ODEvent_od_wt_charge, &b_ODEvent_od_wt_charge);
   fChain->SetBranchAddress("ODEvent.od_cluster_charge", ODEvent_od_cluster_charge, &b_ODEvent_od_cluster_charge);
   fChain->SetBranchAddress("ODEvent.od_cluster_start", ODEvent_od_cluster_start, &b_ODEvent_od_cluster_start);
   fChain->SetBranchAddress("ODEvent.od_cluster_height", ODEvent_od_cluster_height, &b_ODEvent_od_cluster_height);
   fChain->SetBranchAddress("ODEvent.od_cluster_multiplicity", ODEvent_od_cluster_multiplicity, &b_ODEvent_od_cluster_multiplicity);
   fChain->SetBranchAddress("ODEvent.od_cluster_pass_multcut", ODEvent_od_cluster_pass_multcut, &b_ODEvent_od_cluster_pass_multcut);
   fChain->SetBranchAddress("ODEvent.od_cluster_dtprompt", ODEvent_od_cluster_dtprompt, &b_ODEvent_od_cluster_dtprompt);
}

Bool_t DSTSelfTrigger::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef DSTSelfTrigger_cxx
