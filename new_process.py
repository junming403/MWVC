import os
path = "raw"
all_file = [os.path.join(path, fi) for fi in os.listdir(path)]

for file in all_file:
    file_name = str(file.split(os.sep)[-1])
    print(file_name)
    with open(file) as f:
        x = f.readline().split(" ")
        V = int(x[2])
        E = int(x[3])
        with open ("data/"+file_name, "w") as w:
            w.write(str(V) + " " + str(E) + "\n")
            for i in range(V):
                vtx = f.readline().split(" ")
                w.write(vtx[-1].strip() + " ")
            w.write("\n")
            for i in range(E):
                xx = f.readline().split(" ")
                u = int(xx[1]) - 1
                v = int(xx[2]) - 1
                # print(str(u) + " " + str(v))
                w.write(str(u) + " " + str(v) + "\n")
