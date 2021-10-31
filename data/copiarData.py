import subprocess as sp
def createTests(): 
    for i in range(1,100):
        comando = ["cp", "./merge", "./merge-"+str(i)]
        sp.check_call(comando, encoding="utf8")
def limpiarData():
    comando=["mkdir", "../backup"]
    sp.check_call(comando, encoding="utf8")

    comando=["cp", "test-1", "../backup/test-1"]
    sp.check_call(comando, encoding="utf8")
    
    comando=["cp", "test-2", "../backup/test-2"]
    sp.check_call(comando, encoding="utf8")

    comando=["cp", "test-3", "../backup/test-3"]
    sp.check_call(comando, encoding="utf8")

    comando=["cp", "merge", "../backup/merge"]
    sp.check_call(comando, encoding="utf8")

    comando=["cp", "corpus", "../backup/corpus"]
    sp.check_call(comando, encoding="utf8")

    comando=["cp", "copiarData.py", "../backup/copiarData.py"]
    sp.check_call(comando, encoding="utf8")

    comando=["rm", "-rf", "../data"]
    sp.check_call(comando, encoding="utf8")

    comando=["mv", "../backup", "../data"]
    sp.check_call(comando, encoding="utf8")

createTests()