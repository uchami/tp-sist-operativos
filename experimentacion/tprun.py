
   
import subprocess as sp
import numpy as np
import random

# configuracion de plots
from matplotlib import pyplot as plt
from matplotlib import rc, rcParams, axes
from matplotlib import axes as mplibAxes
import pandas as pd
# import platform

# import pickle


def tprun(threads_lectura, threads_maximo, amountOfFiles):
    ejecutable = "../build/ContarPalabras"
    comando = [ejecutable, str(threads_lectura), str(threads_maximo)]
    for i in range(1, amountOfFiles):
        comando.append("../data/corpus-" + str(i))
    res = sp.check_output(comando, encoding="utf8")
    [tiempo_archivos, tiempo_max, palabraMax, apariciones] = res.split()
    print([tiempo_archivos, tiempo_max, palabraMax, apariciones])
    return [tiempo_archivos, tiempo_max, palabraMax, apariciones]

def scatterPlot(ejeY, ejeX, color, label):
    plt.figure(figsize=(12,12), dpi= 80)
    
    plt.scatter(ejeX, ejeY, s=30, alpha=1, label=label, color=color)

    # plt.yticks(ejeY)
    # plt.xticks(rotation = -30)
    plt.title('Carga de 30 archivos x Cantidad de threads', fontdict={'size':20})
    plt.xlabel('Cantidad de threads')
    plt.ylabel("Tiempo (s)")
    plt.grid(linestyle='--')
    # plt.xlim(0, 1)
    plt.legend()
    plt.show()
    #esto es para agregarle el valor a los puntitos
    # for x, y, tex in zip(data, data.index, data):
    #     t = plt.text(x, y, tex, horizontalalignment='center', 
    #                 verticalalignment='center', fontdict={'color':'red'})


def experimento_variando_threads():
    tiempos_archivos = []
    tiempos_max = []
    totalThreads = 30 
    for threads in range(1,totalThreads):
        [tiempo_archivos, tiempo_max, _, _] = tprun(threads, threads, totalThreads)
        tiempos_archivos.append(float(tiempo_archivos))
        tiempos_max.append(float(tiempo_max))
    df = pd.DataFrame(data={
        'CantThreads': range(1,totalThreads), 
        'Archivos': tiempos_archivos,
        'Maximo': tiempo_max
    })
    scatterPlot(df.Archivos, df.CantThreads, "red", "A")

experimento_variando_threads()

    