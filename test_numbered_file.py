import sys

def generate_test_file():
    with open('./numbered_lines.txt', 'w') as f:
        for i in range(100):
            print(f"{i:x<100}", file=f)

def check_test_file(filename):
    with open(filename) as f:
        contents = f.read().replace('\n', '').replace('\r', '').replace('x', '')
    
    correct_contents = ''.join(str(i) for i in range(100))
    if contents == correct_contents:
        print("test file intact")
    else:
        print("NOT RIGHT")

if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print(f"Usage: {sys.argv[0]} DOWNLOADED_FILE")
    check_test_file(sys.argv[1])
