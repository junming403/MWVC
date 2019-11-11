import os

# format: ./a.out [file index] [alpha] [dy_choose_mode]
os.system("g++ -g -O2 -std=gnu++17 code_for_tuning.cpp")

dataset = [
	"./test_case/frb100-40.txt",
	"./test_case/test1.txt",
	"./test_case/test2.txt",
	"./test_case/test3.txt",
	"./test_case/test4.txt",
	"./test_case/test5.txt",
	"./test_case/test6.txt",
	"./test_case/test7.txt",
	"./test_case/test8.txt",
	"./test_case/test9.txt",
	"./test_case/test10.txt",
]
# dataset = [
# 	"./data/keller5.txt", # 0
# 	"./data/C1000.9.txt", # 1
# 	"./data/C500.9.txt", # 2
# 	"./data/san200_0.9_1.txt", # 3
# 	"./data/san1000.txt", # 4
# 	"./data/C2000.5.txt", # 5
# 	"./data/p_hat1500-2.txt", # 6
# 	"./data/brock800_4.txt", # 7
# 	"./data/frb59-26-1.mis", # 8
# 	"./data/DSJC1000_5.txt", # 9
# 	"./data/MANN_a45.txt", # 10
# 	"./data/C2000.9.txt", # 11
# 	"./data/c-fat500-10.txt", # 12
# 	"./data/gen400_p0.9_75.txt", # 13
# 	"./data/hamming10-4.txt", # 14
# 	"./data/frb100-40.mis", # 15
# ]
repeat_times = 51
optimals = []
parameters = []
for id in range(len(dataset)):
    optimals.append(900000000)
    parameters.append([-1, -1, -1])

optimals.append(-1)
parameters.append([-1, -1, -1])

overall_id = len(dataset)

for alpha in range(2, 21):
    for start in range(5, 46, 5):
        for raw_end in range(3, 21):
            total_median = 0
            end = raw_end * 0.5
            if (end > start):
                end = start
            for id in range(len(dataset)):
                results = []
                for repeat in range(repeat_times):
                    cmd = "./a.out {0} {1} {2} {3}".format(str(id), str(alpha), str(start), str(end))
                    ans = int(os.popen(cmd).readline())
                    results.append(ans)
                results.sort()
                median = results[len(results) // 2]
                total_median += median / len(dataset)

                if (median < optimals[id]):
                    optimals[id] = median
                    parameters[id][0] = alpha
                    parameters[id][1] = start
                    parameters[id][2] = end
            
            if (total_median < optimals[overall_id] or optimals[overall_id] ==  -1):
                optimals[overall_id] = total_median
                parameters[overall_id][0] = alpha
                parameters[overall_id][1] = start
                parameters[overall_id][2] = end
            # print("Checkpoint:" + " overall median = " + str(total_median) + " alpha= " + str(alpha) + " start= " + str(start) + " end= " + str(end) )
            print("CHECKPOINT")
            print("optimals: " + str(optimals), flush=True)
            print("parameters: " + str(parameters), flush=True)
print("FINAL ANSWER")
print("optimals: " + str(optimals), flush=True)
print("parameters: " + str(parameters), flush=True)
