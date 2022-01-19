from matplotlib import pyplot as plt


def process_text(file_name):
    acc = []
    with open(file_name) as f:
        for line in f:
            info = line.split(':')
            time_ = float(info[1][1:])
            acc.append(time_)
    return acc


def main():
    scalar = process_text('scalar-o0.txt')
    sse = process_text('sse-o0.txt')
    avx = process_text('avx-o0.txt')
    fma = process_text('fma-o0.txt')
    assert len(scalar) == len(sse)
    assert len(sse) == len(avx)
    assert len(sse) == len(fma)
    data = [i for i in range(len(scalar))]
    plt.plot(data, scalar, label="Scalar")
    plt.plot(data, sse, label="SSE")
    plt.plot(data, avx, label="AVX")
    plt.plot(data, fma, label="FMA")
    plt.xlabel("# of trial")
    plt.ylabel("Duration")
    plt.title("Performance using with O0")
    plt.legend()
    plt.show()


if __name__ == '__main__':
    main()
