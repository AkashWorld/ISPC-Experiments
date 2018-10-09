import random

def generate_numbers(filename):
    print("Generating 20 million floating point numbers to file: " + filename)
    try:
        file = open(filename, 'w')
    except FileNotFoundError:
        file = open(filename, 'x')
    i = 0
    while i < 20000000:
        rand_numb = random.uniform(0, 8)
        if(rand_numb == 0.0):
            i = i - 1
            continue
        file.write(str(rand_numb) + '\n')
        i = i + 1
    file.close()
    
if __name__ == "__main__":
    generate_numbers("20m_square_root.txt")