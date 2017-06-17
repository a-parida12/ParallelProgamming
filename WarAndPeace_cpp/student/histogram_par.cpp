#include "histogram.h"
#include "names.h"

#include <future>
#include <thread>
#include <vector>
#include <algorithm>

histogram_t ThreadFunction(const std::vector<word_t>& words,int start, int end){
 
 histogram_t local_hist;
 
 for(int i=0; i<NNAMES; i++)
	local_hist[i] =0;
 
 for_each(begin(words)+ start,begin(words)+end,[&local_hist](const word_t &single_word )
	{
		int res = getNameIndex(single_word.data());
		if (res != -1)
			local_hist[res]++;
	});
	return local_hist;

 }


void get_histogram(const std::vector<word_t>& words, histogram_t& histogram, int num_threads)
{

 std::vector<std::future<histogram_t>> future_histogram;
 int nBlocksThread=words.size()/num_threads;

 for(int i=0;i<num_threads;i++){
	int start=i*nBlocksThread;
	int end=start+nBlocksThread;

	if(i+1==num_threads)	
		end=words.size();
    //gets the future histogram 
    future_histogram.push_back(std::async(std::launch::async, ThreadFunction, std::ref(words), start, end)); 
	}

 for (auto& thread: future_histogram){
		//function will wait until child completes the assigned task and end
		histogram_t get_thread_histogram = thread.get();
		for (int i=0; i < NNAMES; i++){
			histogram[i] += get_thread_histogram[i];
		}
	}
}
