import subprocess as sp
def createTests(): 
    for i in range(1,100):
        comando = ["cp", "./test-1", "./test1-"+str(i)]
        sp.check_call(comando, encoding="utf8")
def limpiarData():
    comando=["rm", "--", "!(", "-name", "corpus", "-o", "-name", "test-1", "-o", "-name", "test-2", "-o", "-name", "test-3" ")"]
    sp.check_call(comando, encoding="utf8")

limpiarData()