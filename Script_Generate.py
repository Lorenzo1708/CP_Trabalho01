import random

dataFile = open ('MallData.csv', 'w')

for i in range(1000000):

    dataFile.write(f'{random.randint(1, 900)},{random.randint(1, 100)}\n')

dataFile.close()
