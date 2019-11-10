import os

# format: ./a.out [file index] [alpha] [dy_choose_mode]
total_benchmark = 16
os.system("g++ -O2 -std=gnu++17 code_for_tuning.cpp")

id = 5
mode = 0
dataset = [
	"./data/keller5.txt", # 0
	"./data/C1000.9.txt", # 1
	"./data/C500.9.txt", # 2
	"./data/san200_0.9_1.txt", # 3
	"./data/san1000.txt", # 4
	"./data/C2000.5.txt", # 5
	"./data/p_hat1500-2.txt", # 6
	"./data/brock800_4.txt", # 7
	"./data/frb59-26-1.mis", # 8
	"./data/DSJC1000_5.txt", # 9
	"./data/MANN_a45.txt", # 10
	"./data/C2000.9.txt", # 11
	"./data/c-fat500-10.txt", # 12
	"./data/gen400_p0.9_75.txt", # 13
	"./data/hamming10-4.txt", # 14
	"./data/frb100-40.mis", # 15
]
repeat_times = 20
optimal = 9000000000
parameters = [-1, -1, -1]
# for alpha in range(1,20):
for dummy in range(3, 20):
    mode = dummy * 0.5
    avg = 0
    id = 5
    alpha = 10
    # for id in range(len(dataset)):
    #     print("process file " + str(id))
    for repeat_time in range(repeat_times):
        cmd = "./a.out {0} {1} {2}".format(str(id), str(alpha), str(mode))
        ans = int(os.popen(cmd).readline())
        avg += (ans / repeat_times)
    
    if (avg < optimal):
        optimal = avg
        parameters[1] = alpha
        parameters[2] = mode
    print("current round: average = " + str(avg) + " alpha= " + str(alpha) + " mode= " + str(mode))
print("final optimal: average = " + str(optimal) + " alpha= " + str(parameters[1]) + " mode= " + str(parameters[2]))
