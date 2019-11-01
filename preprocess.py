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
                w.write("1 ")
            w.write("\n")
            for i in range(E):
                xx = f.readline().split(" ")
                w.write(xx[1] + " " + xx[2])