from threading import Thread
from tkinter import *
import tkinter as tk
import serial
import time


pot = 0.0
ilu = 0.0
amax = 0.0
amin = 0.0
slivalues = 0
servo_control = 0

# Serial
try:
    arduino = serial.Serial("COM4", 9600, timeout=1)
except:
    print("Error de coneccion con el puerto")


def DatosA():

    # arduino.reset_input_buffer()
    while (True):
        time.sleep(1)
        global servo_control, slivalues
        msg = str(servo_control)+","+str(slivalues)
        # print(msg)
        arduino.write((msg + '\n').encode())
        arduino.flush()
        global datos, pot, ilu, amax, amin
        datos = arduino.readline().decode()
        datos = datos.rstrip('\n')
        print(datos)

        if (datos != None):
            DATASPLIT = datos.split(",")
            isReceive = True
            pot = DATASPLIT[0]
            ilu = DATASPLIT[1]
            amax = DATASPLIT[2]
            amin = int(DATASPLIT[3])



thread = Thread(target=DatosA)
thread.start()

# database

# GUI
root = Tk()
root.title('Incubadora')
root['background'] = 'light green'


def clicked(value):
    global servo_control
    servo_control = value


def slide_values(event):
    global slivalues
    slivalues = slider1.get()
    print


def update_labels(lb1, lb2, lb3, lb4):
    def count():
        texto = str(amax)+'%'
        lb1.config(text=texto)
        texto = str(amin)+'%'
        lb2.config(text=texto)
        texto = str(ilu)+'%'
        lb3.config(text=texto)
        texto = str(pot)+' grados'
        lb4.config(text=texto)
        lb1.after(1000, count)
    count()


titulo = Label(root, text="Interfaz Python",
               font="Roboto 16 bold", width=15, bg='light green')
etiqueta1 = Label(root, text="Configuración de",
                  font="Roboto 14", width=17, anchor=SW, bg='light green')
etiqueta2 = Label(root, text="iluminación máx. o min",
                  font="Roboto 14", width=17, anchor=NW, bg='light green')
etiqueta3 = Label(root, text="Estado de iluminación:",
                  font="Roboto 14", width=17, anchor=W, bg='light green')
etiqueta4 = Label(root, text="Ángulo de motor:",
                  font="Roboto 14", width=17, anchor=W, bg='light green')

ilumMax = Label(root, font="Roboto 14",
                bg="yellow", width=12, borderwidth=5)
ilumMin = Label(root, text="40%", font="Roboto 14",
                bg="yellow", width=12, borderwidth=5)

ilumState = Label(root, text="87%", font="Roboto 14",
                  bg="cyan", width=12, borderwidth=5)
angState = Label(root, text="30 Grados", font="Roboto 14",
                 bg="cyan", width=14, borderwidth=5)

r = IntVar()
r.set(0)
check1 = Radiobutton(root, text="Potenciómetro", font="Roboto 14", width=15, indicatoron=0,
                     variable=r, value=1, command=lambda: clicked(r.get()), anchor=W)
check2 = Radiobutton(root, text="Slider", font="Roboto 14", width=15, indicatoron=0,
                     variable=r, value=0, command=lambda: clicked(r.get()), anchor=W)
slider1 = Scale(root, from_=0, to=180,  length=300,
                border=2, orient=HORIZONTAL, command=slide_values)

titulo.grid(row=0, column=0, columnspan=2, padx=10, pady=10)
etiqueta1 .grid(row=1, column=0, padx=10, pady=0, sticky=S)
etiqueta2 .grid(row=2, column=0, padx=10, pady=0, sticky=N)
etiqueta3 .grid(row=3, column=0, padx=10, pady=10)
etiqueta4 .grid(row=4, column=0, padx=10, pady=10)

ilumMax.grid(row=1, column=1, padx=10, pady=2, sticky=W)
ilumMin.grid(row=2, column=1, padx=10, pady=2, sticky=W)
ilumState.grid(row=3, column=1, padx=10, pady=10, sticky=W)
angState.grid(row=4, column=1, padx=10, pady=10, sticky=W)

check1.grid(row=5, column=0, padx=10, pady=2, sticky=E)
check2.grid(row=6, column=0, padx=10, pady=2, sticky=E)

slider1.grid(row=7, column=0, columnspan=2, padx=10, pady=10)

update_labels(ilumMax, ilumMin, ilumState, angState)

check1.deselect()
check2.select()

root.mainloop()
arduino.close()