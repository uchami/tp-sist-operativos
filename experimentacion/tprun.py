
   
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
    iteraciones = 10
    ejecutable = "../build/ContarPalabras"
    comando = [ejecutable, str(threads_lectura), str(threads_maximo)]
    characters = 'abcdefghijklmnñopqrstuvwxyz'
    for i in range(0, amountOfFiles):
        comando.append("../data/palabras_" + characters[i] + ".txt")
    print(comando)
    promedio_archivos = []
    promedio_max = []
    for i in range(1,iteraciones):
        res = sp.check_output(comando, encoding="utf8")
        [tiempo_archivos, tiempo_max, _, _] = res.split()
        promedio_archivos.append(float(tiempo_archivos))
        promedio_max.append(float(tiempo_max))
    return [np.ma.median(promedio_archivos), np.ma.median(promedio_max)]

def scatterPlot(ejeY, ejeX, color, label):
    plt.figure(figsize=(12,12), dpi= 80)
    
    plt.scatter(ejeX, ejeY, s=30, alpha=1, color=color)

    # plt.yticks(ejeY)
    plt.xticks(fontsize=15)
    plt.yticks(fontsize=15)
    plt.title(label, fontdict={'size':20})
    plt.xlabel('Cantidad de threads', fontsize=20)
    # plt.set('FontSize', 30)
    plt.ylabel("Tiempo (s)", fontsize=20)
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
    totalThreads = 26
    for threads in range(1,totalThreads):
        [tiempo_archivos, tiempo_max] = tprun(5, threads, 13)
        tiempos_archivos.append(float(tiempo_archivos))
        tiempos_max.append(float(tiempo_max))
    df = pd.DataFrame(data={
        'CantThreads': range(1,totalThreads), 
        'Archivos': tiempos_archivos,
        'Maximo': tiempos_max
    })
    scatterPlot(df.Maximo, df.CantThreads, "red", "Calculo del maximo en paralelo para diccionario")

experimento_variando_threads()

    