import sys
from matplotlib import pyplot as plt


def process_text(file_name):
    with open(file_name) as f:
        line = f.read()
        time_list = line.split(',')[:-1]  # exclude last element which is empty string
    return [float(item) for item in time_list]


def main():
    iterations = int(sys.argv[1])
    
    scalar = process_text('scalar.txt')
    sse = process_text('sse.txt')
    fma = process_text('fma.txt')
    assert len(scalar) == len(sse)
    assert len(sse) == len(fma)

    x = [i for i in range(len(scalar))]
    
    plt.plot(x, scalar, label="Scalar")
    plt.plot(x, sse, label="SSE")
    plt.plot(x, fma, label="FMA")
    plt.xlabel("# of trials")
    plt.ylabel("Time(s)")
    plt.title("Simple Vectorizations at {} iterations".format(iterations))
    plt.legend()
    plt.savefig('simple-{}.png'.format(iterations))


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Please give iteration argument. For ex: python3 plot.py 100000000")
        sys.exit(1)
    main()
