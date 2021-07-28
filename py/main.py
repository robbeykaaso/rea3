import sys
from PyQt5.QtWidgets import QApplication, QWidget
import test

def main():

    app = QApplication(sys.argv)

    #test.test0()
    #test.test1()
    #test.test2()

    # w = QWidget()
    # w.resize(250, 150)
    # w.move(300, 300)
    # w.setWindowTitle('Simple')
    # w.show()

    sys.exit(app.exec_())

if __name__ == '__main__':
    main()