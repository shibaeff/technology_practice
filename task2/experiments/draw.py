import random
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from collections import defaultdict

class Record:
    def __init__(self, line):
        if line == "":
            self.cpus = 0
            self.jobs = 0
            self.threads = 0
            self.time = 0
            self.start_criteria = 0
            self.final_criteria = 0
            return
        values = []
        for item in line.split(","):
            _, value = item.split(":")
            values.append(value)
        self.cpus = int(values[0])
        self.jobs = int(values[1])
        self.threads = int(values[2])
        self.time = float(values[3])
        self.start_criteria = float(values[4])
        try:
            self.final_criteria = float(values[5])
        except:
            self.final_criteria = float(values[5].split()[0])

    def to_dict(self):
        return self.__dict__


def get_pandas1(path):
    d = defaultdict(int)
    with open(path, "r") as f:
        instances = []
        for line in f.readlines():
            r = Record(line)
            d[(r.cpus, r.jobs)] = (r.time + d[(r.cpus, r.jobs)]) * 0.5

        s = pd.Series(d)
        df = s.unstack()
        return df


def get_pandas2(paths):
    d = defaultdict(float)
    for path in paths:
        name = path.split("_")[-1]
        with open(path, "r") as f:
            for line in f.readlines():
                r = Record(line)
                v = d[((r.cpus, r.jobs), name)]
                v = (v + r.time) * 0.5
                d[((r.cpus, r.jobs), name)] = v

    s = pd.Series(d, index=pd.MultiIndex.from_tuples(d))
    df = s.unstack()
    return df


def get_pandas3(path):
    d = defaultdict(float)
    with open(path, "r") as f:
            for line in f.readlines():
                r = Record(line)
                v = d[r.threads]
                v = (v + r.time) * 0.5
                d[r.threads] = v
    sorted_keys = sorted(d.keys())
    sorted_dict = {key: d[key] for key in sorted_keys}
    df=pd.DataFrame(list(sorted_dict.items()), columns=['Threads', 'Execution Time'])
    return df


def get_pandas4(path):
    d = defaultdict(float)
    with open(path, "r") as f:
            for line in f.readlines():
                r = Record(line)
                v = d[r.threads]
                v = (v + r.final_criteria) * 0.5
                d[r.threads] = v
    sorted_keys = sorted(d.keys())
    sorted_dict = {key: d[key] for key in sorted_keys}
    df=pd.DataFrame(list(sorted_dict.items()), columns=['Threads', 'Quality'])
    return df


def image1():
    df = get_pandas1("./logs_1")
    heatmap_data = df
    sns.heatmap(heatmap_data, annot=True, fmt=".2f")
    # plt.figure(figsize=(10, 8))
    plt.title('Время выполнения алгоритма, с')
    plt.xlabel('Количество ПУ')
    plt.ylabel('Количество работ')
    plt.savefig('image1.jpg', dpi=600, bbox_inches='tight')


def image2():
    df = get_pandas2(["./logs_2_boltzman", "./logs_2_couchy", "./logs_2_log"])
    heatmap_data = df
    sns.heatmap(heatmap_data, annot=True, fmt=".2f")
    # plt.figure(figsize=(10, 8))
    plt.title('Время выполнения алгоритма, с')
    plt.xlabel('Количество ПУ')
    plt.ylabel('Количество работ')
    plt.savefig('image2.jpg', dpi=600, bbox_inches='tight')


def image3():
    df = get_pandas3("./logs_3_new")
    plt.title("Зависимость времени выполнения от количества потоков")
    plt.plot(df['Threads'], df['Execution Time'])
    plt.savefig('image3.jpg', dpi=600, bbox_inches='tight')


def image4():
    df = get_pandas4("./logs_3_new")
    plt.title("Зависимость критерия от количества потоков")
    plt.plot(df['Threads'], df['Quality'])
    plt.savefig('image4.jpg', dpi=600, bbox_inches='tight')



image1()
plt.close()
image2()
plt.close()
image3()
plt.close()
image4()
