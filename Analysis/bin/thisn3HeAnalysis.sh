n3HeAnalysisBIN=$(dirname $BASH_ARGV[0])
n3HeAnalysisROOT=$(cd $n3HeAnalysisBIN/..;pwd)

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$n3HeAnalysisROOT/lib
export PATH=$PATH:$n3HeAnalysisROOT/bin


