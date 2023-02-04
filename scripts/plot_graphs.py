import matplotlib.pyplot as plt
import os
import sys

def plot_line(xs, ys, algorithm, algo_option):
    plt.plot(xs, ys, marker = 'o', label = algo_option)
    plt.ylabel('Time (sec)')
    plt.xlabel('No. of Points')
    plt.title(algorithm, fontsize = 10)
    plt.legend(loc="best")


def plot_lines(algorithm, all_points, labels, subplots=False):
    for idx, line_points in enumerate(all_points):
        line_xs = line_points[0]
        line_ys = line_points[1]
        line_xs, line_ys = zip(*sorted(zip(line_xs, line_ys)))
        plot_line(line_xs, line_ys, algorithm, labels[idx])

def read_points(directory):
    labels = ["Random Edge", "Min Area Edge", "Max Area Edge"]  
    if directory.find("onion") != -1:
        algorithm = "Onion Algorithm"
        labels = ["Random Point", "Min X Point", "Max X Point", "Min Y Point", "Max Y Point"]
    elif directory.find("convex_hull") != -1:
        algorithm = "Convex Hull to Polygon Line Algorithm"
    else:
        if directory[-2:] == "1a":
            algorithm = "Incremental Algorithm (Descending X)"
        elif directory[-2:] == "1b":
            algorithm = "Incremental Algorithm (Ascending X)"
        elif directory[-2:] == "2a":
            algorithm = "Incremental Algorithm (Descending Y)"
        elif directory[-2:] == "2b":
            algorithm = "Incremental Algorithm (Ascending Y)"
    all_points = list()
    plt.figure(figsize=(6, 4), dpi=65)
    for filename in os.listdir(directory):
        cur_file_xs = list()
        cur_file_ys = list()
        with open(os.path.join(directory, filename), 'r') as f: # open in readonly mode
            for line in f:
                x = line.split()[0]
                y = line.split()[1]
                cur_file_xs.append(int(x))
                y = y.replace(',', '.')
                cur_file_ys.append(float(y))
        all_points.append((cur_file_xs, cur_file_ys))
        
    plot_lines(algorithm, all_points, labels)
    plt.show()


def walk_directory(directory):
    for item in os.listdir(directory):
        if os.path.isdir(f"{directory}/{item}"): # recursively visit this dir
            walk_directory(f"{directory}/{item}")
        else: # read points from all files in this directory
            read_points(directory)
            return

if __name__ == '__main__':   
    directory = sys.argv[1]
    walk_directory(directory)