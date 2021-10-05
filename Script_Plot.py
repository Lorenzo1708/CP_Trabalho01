import matplotlib.pyplot
import pandas
import seaborn

# Before Clustering
data = pandas.read_csv('MallData.csv', header=None)

data.columns = ['Annual Income (k$)', 'Spending Score (1-100)']

seaborn.scatterplot(x=data['Annual Income (k$)'], y=data['Spending Score (1-100)'])

matplotlib.pyplot.title('Scatterplot of Spending (Y) versus Income (X)')

matplotlib.pyplot.savefig('MallData.png')

noThreads = [1, 2, 4, 8]

for noThread in noThreads:

    # After Clustering
    matplotlib.pyplot.figure()

    fileName = f'Output_{noThread}_Thread'

    if noThread != 1:

        fileName = f'Output_{noThread}_Threads'

    data = pandas.read_csv(f'{fileName}.csv')

    seaborn.scatterplot(x=data.X, y=data.Y, hue=data.C, palette=seaborn.color_palette('hls', n_colors=5))

    matplotlib.pyplot.xlabel('Annual Income (k$)')

    matplotlib.pyplot.ylabel('Spending Score (1-100)')

    matplotlib.pyplot.title('Clustered: Spending (Y) versus Income (X)')

    matplotlib.pyplot.savefig(f'{fileName}.png')
