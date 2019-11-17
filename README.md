
# Group 11 Members

Marius Jonika (s174411)

Jack William Marshall Rose (s164559)

Weston Jones (s191380)

Sumanth Varambally (s191562)

Alexandros Spyropoulos (s182346)

Nicolai Oliver Verbaarschot (s155932)

Andreas Klit Bundgaard Jense (s161765)

# Experiments

## Experiment 1: Caching (Weston)

### Hypothesis

After testing our program against various client configurations, it appears that the majority of runtime is spent doing “reverse hashing” – hashing sequences of numbers until one of the generated hashes matches the client input. One way to shorten this time, since we know that the final client configuration will be set to repeat hashes, is to store the result of a successful reverse hashing run. This way, when the client repeats itself and sends a hash that our server has seen before, it can immediately send back the correct reverse hash rather than repeating the entire reverse hashing process again.

In this experiment I’ll compare a few methods and data structures for storing and retrieving reverse hashes (I’ll refer to this process as caching from now on). First, as a baseline, I’ll look at performance of the program without any sort of caching. Second, I’ll look at a cache that stores correct reverse hashes in an array and uses a linear search for lookup. Third, I’ll use a cache that operates similarly to a hash table – mapping values to indices in an array. 

I suspect that caching will provide some sort of performance gain because of the delay argument I explained above, and I believe the hash table will function best. Hash tables can do lookup in constant time, which should make cache retrievals and additions very fast. While they potentially use up more memory than a linear array/list (The table should be initialized to a maximum size right away to avoid having to rehash stored values whereas a linear list can easily increase in size later on), I believe the speed of operations will make up for this shortcoming.
 
### Implementation
	
The implementation of the “baseline” version consisted of the current working version of our server at the time of experimentation, basically the “FIFO” Scheduler found under the experiments folder. The other caching methods for this experiment build off of this “baseline” as well. 
	
For the linear list, the cache consists of an initially empty array of size XXX storing key-value pairs and an integer showing the current size of the array. The key-value pairs consist of a key – the hash the server receives from the client – and a value – the reverse hash that the server normally sends back to the client. 
Upon receiving a hash from a client, the server does a linear search of the cache – comparing the received hash against each key in the cache. If a match is found, the value at that index in the cache is treated as the correct reverse hash and sent back to the client. If a match is not found, reverse hashing proceeds as normal and the corresponding key-value pair is added to the end of the list (this is found by referencing and then incrementing the cache size variable).

For the hash table cache, the basic structure and operation of the cache is kept the same (the main data structure is an array of size XXX with each index containing a key-value pair), with a few key differences. First the size variable is done away with because it is no longer relevant to this implementation. Second and more importantly, all cache operations are done in constant time by using the first five bytes of a hash to compute the index that it belongs in. 

### Comparison

To effectively compare these caching methods, I used a custom client. The goal was to provide a substantial challenge to push each implementation to its limits while also allowing each test to be completed quickly so that more holistic results and averages could be obtained and examined. These results are displayed below in tabular format. 
Trial Number	No Cache / Baseline	Cache With Linear Search	Cache with Hash table
1			
2			
3			
4			
5			
Average			
	
### Conclusion

## Other Experiment: Marius 

* Comparing the speed of multiprocessing and multithreading
* Comparing speed of regular multiprocessing with multiprocessing on top of FIFO scheduling
