import statistics
from matplotlib import pyplot as plt


def process_text(file_name):
    with open(file_name) as f:
        line = f.read()
        # exclude last element which is empty string
        time_list = line.split(',')[:-1]
    return [float(item) for item in time_list]


def main():
    scalar = process_text('loop-vectorize/scalar.txt')
    vector = process_text('loop-vectorize/vector.txt')
    if_scalar = process_text('if-vectorize/scalar.txt')
    if_vector = process_text('if-vectorize/vector.txt')
    assert len(scalar) == len(vector)
    assert len(if_scalar) == len(if_vector)

    plt.xlabel("Loop Type")
    plt.ylabel("Median Time(s)")
    plt.title("Loop-Vectorize with loop length 100M")

    x = ["Scalar", "Vectorized", "Scalar with if",
         "Vectorized with if"]
    y = [statistics.median(scalar), statistics.median(
        vector), statistics.median(if_scalar), statistics.median(if_vector)]
    plt.bar(x, y)
    # generate text on bars
    [plt.text(x[i], y[i], round(y[i], 6), ha='center') for i in range(len(x))]

    plt.savefig('result.png')


if __name__ == '__main__':
    main()
