from threading import Thread
import tkinter
import tkinter.messagebox
import customtkinter
import serial
import time
import cv2
import numpy as np
import imutils
import os
from PIL import Image, ImageTk
valx = 0
valy = 0
valz = 0
vale = 0
valb = 0

comando=0
posx=0
posy=0
posz=0
pose=0

customtkinter.set_appearance_mode("Light")  # Modes: "System" (standard), "Dark", "Light"
customtkinter.set_default_color_theme("blue")  # Themes: "blue" (standard), "green", "dark-blue"

try:
    #arduino = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
    arduino = serial.Serial("COM9", 9600, timeout=1)
except:
    print("Error de coneccion con el puerto")

def recibirDatos():

    while (True):
        time.sleep(1)
        global datos, comando, posx, posy, posz, pose
        datos = arduino.readline().decode()
        datos = datos.rstrip('\n')
        #print(datos)

        if (datos != ""):
            DATASPLIT = datos.split(",")
            print(DATASPLIT)
            isReceive = True
            comando = (DATASPLIT[0])
            posx = (DATASPLIT[1])
            posy = (DATASPLIT[2])
            posz = (DATASPLIT[3])
            pose = (DATASPLIT[4])
            
    
        actual_posiciones()
        proceso_actual()


        
def actual_posiciones():
    global valx,valy,valz,vale
    if float(comando)==3 and posx!= "" and posy!= "" and posz!= "" and pose!= "":
        valx=float(posx)
        valy=float(posy)
        valz=float(posz)
        vale=float(pose)
        text_posicion.set(f" Posición x : {(posx)}\n Posición y : {(posy)}\n Posición z : {(posz)}\n Posición e : {(pose)}")


def proceso_actual():
    global posx
    if float(comando)==4:
        print(comando)
        match float(posx):
            case 3:
                print("Proceso de Corte")
                posx=0
            case 4:
                print("Proceso de Lijado")
                posx=0
            case 5:
                print("Proceso de Pulido")
                posx=0


thread = Thread(target=recibirDatos)
thread.start()
      

def sliderpasos_event(valuepasos):
    global valpasos
    valpasos=valuepasos
    text_pasos.set(f"{int(slider_pasos.get())}")
    #print(valx)


def envio_pasos():
    dato="3,"+str(valx)+","+str(valy)+","+str(valz)+","+str(vale)+","+str(valb)
    print(dato)
    arduino.write((dato + '\n').encode())
    arduino.flush()


def incremento_x():
    global valx
    valx=valx+valpasos;
    envio_pasos()

def decremento_x():
    global valx
    valx=valx-valpasos;
    envio_pasos()

def incremento_y():
    global valy
    valy=valy+valpasos;
    envio_pasos()

def decremento_y():
    global valy
    valy=valy-valpasos;
    envio_pasos()

def incremento_z():
    global valz
    valz=valz+valpasos;
    envio_pasos()

def decremento_z():
    global valz
    valz=valz-valpasos;
    envio_pasos()

def incremento_e():
    global vale
    vale=vale+(valpasos*100);
    envio_pasos()

def decremento_e():
    global vale
    vale=vale-(valpasos*100);
    envio_pasos()

def incremento_b():
    global valb
    valb=valb+(valpasos*100);
    envio_pasos()

def decremento_b():
    global valb
    valb=valb-(valpasos*100);
    envio_pasos()

def reset_ejes():
    global valx,valy,valz
    valx=0
    valy=0
    valz=0

def homming():
    dato="4"
    arduino.write((dato + '\n').encode())
    arduino.flush()
    print("SetHome")

def go_home():
    global valx,valy,valz,vale
    valx=0
    valy=0
    valz=0
    vale=0
    dato="3,0,0,0,0"
    print(dato)
    arduino.write((dato + '\n').encode())
    arduino.flush()
    


def boton1():
    msg = "1"
    arduino.write((msg + '\n').encode())
    arduino.flush()

def boton2():
    msg = "2"
    arduino.write((msg + '\n').encode())
    arduino.flush()     

def material1():
    global material
    material= "1"
    #print(material)

def material2():
    global material
    material= "2"
    #print(material)

def material3():
    global material
    material= "3"
    #print(material)


def diametro1():
    global diametro
    diametro= "1"
    #print(diametro)

def diametro2():
    global diametro
    diametro= "2"
    #print(diametro)

def diametro3():
    global diametro
    diametro= "3"
    #print(diametro)

def sidebar_button_event():
    print("sidebar_button click")

def enviar_seleccion():
    global material,diametro
    if material == "1":
        print("Acero")
        mate="1"
    elif material == "2":
        print("Aluminio")
        mate="2"
    elif material == "3":
        print("Cobre")
        mate="3"

    if diametro == "1":
        print("12mm")
        dia="1"
    elif diametro == "2":
        print("15mm")
        dia="2"
    elif diametro == "3":
        print("20mm")
        dia="3"

    if check_proceso_1.get() == "on":
        print("Corte")
        corte="1"
    else:
        corte="0"
    if check_proceso_2.get() == "on":
        print("Lijado")
        lijado="1"
    else:
        lijado="0"
    if check_proceso_3.get() == "on":
        print("Pulido")
        pulido="1"
    else:
        pulido="0"
    if check_proceso_4.get() == "on":
        print("Revision")
        inspec="1"
    else:
        inspec="0"

    selec="2,"+mate+","+dia+","+corte+","+lijado+","+pulido+","+inspec
    print(selec)
    arduino.write((selec + '\n').encode())
    arduino.flush()


def checkbox_event ():
    pass

def state_amoladora():
    if switch_am.get() == "on":
        dato="5"
        print("Amo Encendida")
        arduino.write((dato + '\n').encode())
        arduino.flush()
    elif switch_am.get() == "off":
        dato="6"
        print("Amo Apagado")
        arduino.write((dato + '\n').encode())
        arduino.flush()

def state_lijas():
    if switch_li.get() == "on":
        dato="7"
        print("Lija Encendida")
        arduino.write((dato + '\n').encode())
        arduino.flush()
    elif switch_li.get() == "off":
        dato="8"
        print("Lija Apagado")
        arduino.write((dato + '\n').encode())
        arduino.flush()

def state_pulidora():
    if switch_puli.get() == "on":
        global valb
        valb=valb+(2*100);
        envio_pasos()


def slidervel_event(valuevel):
    global valvel
    valvel=valuevel
    dato="9,"+str(valvel)
    arduino.write((dato + '\n').encode())
    arduino.flush()

def salir_interfaz():
    app.destroy()

def apagar_raspberry():
    os.system("sudo shutdown -h now") 

################
def FindScratches(img):
    # Load the image
    image = img

    # Convert the image to grayscale
    gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)

    # Apply Gaussian blur to reduce noise
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    edges = blurred
    # Use the Hough Circle Transform to detect circles in the image
    circles = cv2.HoughCircles(blurred, cv2.HOUGH_GRADIENT, dp=1, minDist=20, param1=50, param2=30, minRadius=200, maxRadius=240)

    scratches = 0

    if circles is not None:
        circles = np.uint16(np.around(circles))

        # Initialize variables to track the largest circle and its area
        largest_circle = None
        largest_area = 0

        for circle in circles[0, :]:
            center = (circle[0], circle[1])
            radius = circle[2]
            cv2.circle(image, center, radius, (0, 255, 0), 2)

            # Calculate the area of the circle
            circle_area = np.pi * (radius ** 2)

            # Check if the current circle has a larger area than the largest found so far
            if circle_area > largest_area:
                largest_area = circle_area
                largest_circle = (center, radius)

        # Draw the largest circle in red
        if largest_circle is not None:
            center, radius = largest_circle
            cv2.circle(image, center, radius, (0, 0, 255), 2)

            # Define the circular region of the largest circle as a sub-image
            x, y = center[0] - radius, center[1] - radius
            w, h = 2 * radius, 2 * radius

            # Create an empty black image
            mask = np.zeros_like(gray)
            # Create a circular mask by drawing a filled white circle
            cv2.circle(mask, (center[0], center[1]), radius, (255, 255, 255), -1)

            # Apply the circular mask to the image
            circular_region = cv2.subtract(mask, gray)
            circular_region = circular_region[y:y + h, x:x + w]

            # Apply edge detection within the circular region
            edges = cv2.Canny(circular_region, 50, 150)

            # Find contours in the circular region
            contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

            # Minimum contour length to consider as a scratch within the circular region
            min_contour_length = 1

            # Iterate through the contours and filter out small ones (potential scratches)
            for contour in contours:
                if len(contour) > min_contour_length:
                    # Draw a green rectangle around the detected scratch within the circular region
                    x, y, w, h = cv2.boundingRect(contour)
                    cv2.rectangle(image, (x + center[0] - radius, y + center[1] - radius), (x + w + center[0] - radius, y + h + center[1] - radius), (0, 255, 0), 2)
                    scratches = scratches + 1

        # Save the image with scratch detection, circle detection, and the largest circle
        #cv2.imwrite('image_with_scratch_circle_and_largest_circle.jpg', image)
        #
        return(edges,scratches)
################

video = None

def video_stream():
    global video
    video= cv2.VideoCapture(0)
    iniciar()

def VisionTest():
    ret, frame = video.read()
    if ret==True:
        processImg,N_Scratches = FindScratches(frame)
        print('Scratches find:' + str(N_Scratches))

        processImg=imutils.resize(processImg,width=200)
        processImg=cv2.cvtColor(processImg, cv2.COLOR_BGR2RGB)
        img = Image.fromarray(processImg)
        rev = ImageTk.PhotoImage(image=img)
        #rev = customtkinter.CTkImage(dark_image=img,size=(300,300))
        imagen_box.configure(image=rev)
        imagen_box.image = rev
        imagen_box.after(10,VisionTest)

    
def iniciar():
    global video
    ret, frame = video.read()
    if ret==True:
        frame=imutils.resize(frame,width=250)
        frame=cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        img = Image.fromarray(frame)
        camara = ImageTk.PhotoImage(image=img)
        #camara = customtkinter.CTkImage(dark_image=img,size=(300,300))
        video_box.configure(image=camara)
        video_box.image = camara
        video_box.after(10,iniciar)

app = customtkinter.CTk()
app.wm_attributes('-fullscreen', True)
app.title("CustomTkinter complex_example.py")
app.geometry(f"{700}x{380}")

tabview = customtkinter.CTkTabview(master=app, width=750,height=400)
tabview.place(relx=0.5,rely=0.5,anchor=tkinter.CENTER)
#tabview.grid(row=0, column=1, padx=(20, 20), pady=(20, 0), sticky="nsew")
tabview.add("Selección Parámetros")
tabview.add("Verificación Muestra")
tabview.add("Control de los ejes")
tabview.tab("Selección Parámetros")  # configure grid of individual tabs
tabview.tab("Verificación Muestra")
tabview.tab("Control de los ejes")


label_radio_group = customtkinter.CTkLabel(tabview.tab("Selección Parámetros"), text="Seleccionar Material")
label_radio_group.place(relx=0.247,rely=0.05,anchor=tkinter.NE)
material_menu = tkinter.IntVar(value=3)
radio_button_1 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"),text="Acero", command=material1,variable=material_menu, value=0)
radio_button_1.place(relx=0.247,rely=0.15,anchor=tkinter.NE)
radio_button_2 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"), text="Aluminio",command=material2,variable=material_menu, value=1)
radio_button_2.place(relx=0.247,rely=0.25,anchor=tkinter.NE)
radio_button_3 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"), text="Cobre",command=material3,variable=material_menu, value=2)
radio_button_3.place(relx=0.247,rely=0.35,anchor=tkinter.NE)

label_radio_group = customtkinter.CTkLabel(tabview.tab("Selección Parámetros"), text="Seleccionar Diametro")
label_radio_group.place(relx=0.547,rely=0.05,anchor=tkinter.NE)
diametro_menu = tkinter.IntVar(value=3)
radio_button_1 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"), text="12 mm",command=diametro1,variable=diametro_menu, value=0)
radio_button_1.place(relx=0.547,rely=0.15,anchor=tkinter.NE)
radio_button_2 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"), text="15 mm",command=diametro2,variable=diametro_menu, value=1)
radio_button_2.place(relx=0.547,rely=0.25,anchor=tkinter.NE)
radio_button_3 = customtkinter.CTkRadioButton(tabview.tab("Selección Parámetros"), text="20 mm",command=diametro3,variable=diametro_menu, value=2)
radio_button_3.place(relx=0.547,rely=0.35,anchor=tkinter.NE)

check_proceso_1= tkinter.StringVar(value="off")
check_proceso_2= tkinter.StringVar(value="off")
check_proceso_3= tkinter.StringVar(value="off")
check_proceso_4= tkinter.StringVar(value="off")

label_radio_group = customtkinter.CTkLabel(tabview.tab("Selección Parámetros"), text="Seleccionar Procesos")
label_radio_group.place(relx=0.847,rely=0.05,anchor=tkinter.NE)
checkbox_1 = customtkinter.CTkCheckBox(master=tabview.tab("Selección Parámetros"),text="Corte",command=checkbox_event,variable=check_proceso_1,onvalue="on",offvalue="off")
checkbox_1.place(relx=0.847,rely=0.15,anchor=tkinter.NE)
checkbox_2 = customtkinter.CTkCheckBox(master=tabview.tab("Selección Parámetros"),text="Lijado",command=checkbox_event,variable=check_proceso_2,onvalue="on",offvalue="off")
checkbox_2.place(relx=0.847,rely=0.28,anchor=tkinter.NE)
checkbox_3 = customtkinter.CTkCheckBox(master=tabview.tab("Selección Parámetros"),text="Pulido",command=checkbox_event,variable=check_proceso_3,onvalue="on",offvalue="off")
checkbox_3.place(relx=0.847,rely=0.41,anchor=tkinter.NE)
checkbox_4 = customtkinter.CTkCheckBox(master=tabview.tab("Selección Parámetros"),text="Inspeccion",command=checkbox_event,variable=check_proceso_4,onvalue="on",offvalue="off")
checkbox_4.place(relx=0.847,rely=0.54,anchor=tkinter.NE)

sidebar_button_1 = customtkinter.CTkButton(tabview.tab("Selección Parámetros"),text="Detener", command=boton1)
sidebar_button_1.place(relx=0.747,rely=0.8,anchor=tkinter.NE)

sidebar_button_2 = customtkinter.CTkButton(tabview.tab("Selección Parámetros"),text="Iniciar", command=enviar_seleccion)
sidebar_button_2.place(relx=0.947,rely=0.8,anchor=tkinter.NE)


# textbox = customtkinter.CTkTextbox(tabview.tab("Selección Parámetros"), width=250)
# textbox.grid(row=5, column=0, padx=(20, 0), pady=(20, 0), sticky="nsew")

# textbox = customtkinter.CTkTextbox(tabview.tab("Verificación Muestra"), width=250)
# textbox.grid(row=0, column=0, padx=(20, 0), pady=(20, 0), sticky="nsew")

# titulo_switch = customtkinter.CTkLabel(tabview.tab("Verificación Muestra"), text="Manual Automatico")
# titulo_switch.grid(row=0, column=1, columnspan=1, padx=10, pady=10, sticky="")
# switch = customtkinter.CTkSwitch(tabview.tab("Verificación Muestra"), text=" ")
# switch.grid(row=0, column=3, padx=10, pady=(0, 20))

# boton_acep = customtkinter.CTkButton(tabview.tab("Verificación Muestra"),text="Detener")
# boton_acep.grid(row=1, column=1, padx=20, pady=10)

# boton_rech = customtkinter.CTkButton(tabview.tab("Verificación Muestra"),text="Iniciar")
# boton_rech.grid(row=1, column=3, padx=20, pady=10)

# textbox = customtkinter.CTkTextbox(tabview.tab("Control de los ejes"), width=200)
# textbox.grid(row=0, column=0, padx=(20, 0), pady=(20, 0), sticky="nsew")


    

video_box = customtkinter.CTkLabel(tabview.tab("Verificación Muestra"), width=200,height=100,text="")
video_box.place(relx=0.37,rely=0.1,anchor=tkinter.NE)

act_video = customtkinter.CTkButton(tabview.tab("Verificación Muestra"),text="Activar Camara",command=video_stream)
act_video.place(relx=0.257,rely=0.7,anchor=tkinter.NE)

tomar_foto = customtkinter.CTkButton(tabview.tab("Verificación Muestra"),text="Tomar Foto",command=VisionTest)
tomar_foto.place(relx=0.557,rely=0.8,anchor=tkinter.NE)


titulo_posi = customtkinter.CTkLabel(tabview.tab("Control de los ejes"), text="Posiciones de los Ejes",font=("Arial",20))
titulo_posi.place(relx=0.327,rely=0.05,anchor=tkinter.NE)

text_posicion=tkinter.StringVar(value="Estableza el home \n para definir \n las posiciones")
posicion_box = customtkinter.CTkLabel(tabview.tab("Control de los ejes"), width=130,height=90,font=("Arial",17),textvariable=text_posicion)
posicion_box.place(relx=0.247,rely=0.15,anchor=tkinter.NE)


imagen_box = customtkinter.CTkLabel(tabview.tab("Verificación Muestra"), width=200,height=100,text="")
imagen_box.place(relx=0.67,rely=0.1,anchor=tkinter.NE)

set_home = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Set Home", command=homming)
set_home.place(relx=0.957,rely=0.85,anchor=tkinter.NE)
    
go_tohome = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Go to Home", command=go_home)
go_tohome.place(relx=0.727,rely=0.85,anchor=tkinter.NE)

y_pos = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Y Pos",width=60,height=25,command=incremento_y)
y_pos.place(relx=0.707,rely=0.1,anchor=tkinter.NE)
y_neg = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Y Neg",width=60,height=25,command=decremento_y)
y_neg.place(relx=0.707,rely=0.3,anchor=tkinter.NE)
x_pos = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="X Pos",width=60,height=25,command=incremento_x)
x_pos.place(relx=0.807,rely=0.2,anchor=tkinter.NE)
x_neg = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="X Neg",width=60,height=25,command=decremento_x)
x_neg.place(relx=0.607,rely=0.2,anchor=tkinter.NE)
z_pos = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Z Pos",width=60,height=25,command=incremento_z)
z_pos.place(relx=0.927,rely=0.3,anchor=tkinter.NE)
z_neg = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Z Neg",width=60,height=25,command=decremento_z)
z_neg.place(relx=0.927,rely=0.1,anchor=tkinter.NE)
e_pos = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="E Pos",width=60,height=25,command=incremento_e)
e_pos.place(relx=0.807,rely=0.43,anchor=tkinter.NE)
e_neg = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="E Neg",width=60,height=25,command=decremento_e)
e_neg.place(relx=0.607,rely=0.43,anchor=tkinter.NE)
# b_pos = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="B Pos",width=60,height=25,command=incremento_b)
# b_pos.place(relx=0.947,rely=0.9,anchor=tkinter.NE)
# b_neg = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="B Neg",width=60,height=25,command=decremento_b)
# b_neg.place(relx=0.947,rely=0.9,anchor=tkinter.NE)


slider_inicial= tkinter.IntVar(value=0)

pasos = customtkinter.CTkLabel(tabview.tab("Control de los ejes"), text="Cantidad Pasos")
pasos.place(relx=0.657,rely=0.57,anchor=tkinter.NE)
slider_pasos = customtkinter.CTkSlider(tabview.tab("Control de los ejes"),from_=0,to=50,variable=slider_inicial, command=sliderpasos_event)
slider_pasos.place(relx=0.847,rely=0.67,anchor=tkinter.NE)

text_pasos=tkinter.StringVar(value="")
pasos_box = customtkinter.CTkLabel(tabview.tab("Control de los ejes"), width=60,height=25,textvariable=text_pasos)
pasos_box.place(relx=0.947,rely=0.6,anchor=tkinter.NE)


slider_vel= tkinter.IntVar(value=0)

velocidad = customtkinter.CTkLabel(tabview.tab("Control de los ejes"), text="Velocidad de las lijas")
velocidad.place(relx=0.207,rely=0.6,anchor=tkinter.NE)
slider_velo = customtkinter.CTkSlider(tabview.tab("Control de los ejes"),from_=0,to=100,variable=slider_vel, command=slidervel_event)
slider_velo.place(relx=0.34,rely=0.7,anchor=tkinter.NE)



switch_am=customtkinter.StringVar(value="off")

titulo_amoladora = customtkinter.CTkLabel(tabview.tab("Control de los ejes"), text="Amoladora")
titulo_amoladora.place(relx=0.15,rely=0.42,anchor=tkinter.NE)
switch_amoladora = customtkinter.CTkSwitch(tabview.tab("Control de los ejes"), text=" ",command=state_amoladora,variable=switch_am,onvalue="on",offvalue="off")
switch_amoladora.place(relx=0.2,rely=0.5,anchor=tkinter.NE)


switch_li=customtkinter.StringVar(value="off")

titulo_lijas = customtkinter.CTkLabel(tabview.tab("Control de los ejes"), text="Lijas")
titulo_lijas.place(relx=0.24,rely=0.42,anchor=tkinter.NE)
switch_lijas = customtkinter.CTkSwitch(tabview.tab("Control de los ejes"), text=" ",command=state_lijas,variable=switch_li,onvalue="on",offvalue="off")
switch_lijas.place(relx=0.34,rely=0.5,anchor=tkinter.NE)


switch_puli=customtkinter.StringVar(value="off")

titulo_puli = customtkinter.CTkLabel(tabview.tab("Control de los ejes"), text="Pulidora")
titulo_puli.place(relx=0.4,rely=0.42,anchor=tkinter.NE)
switch_puli = customtkinter.CTkSwitch(tabview.tab("Control de los ejes"), text=" ",command=state_pulidora,variable=switch_puli,onvalue="on",offvalue="off")
switch_puli.place(relx=0.47,rely=0.5,anchor=tkinter.NE)


apagar_rasp = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Apagar Raspberry",fg_color="red",hover_color="red" ,command=apagar_raspberry)
apagar_rasp.place(relx=0.207,rely=0.85,anchor=tkinter.NE)
    
exit_interfaz = customtkinter.CTkButton(tabview.tab("Control de los ejes"),text="Salir de la Interfaz", fg_color="red",hover_color="red",command=salir_interfaz)
exit_interfaz.place(relx=0.427,rely=0.85,anchor=tkinter.NE)

app.mainloop()
arduino.close()