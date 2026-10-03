import math
import sys

class Pixel:
    def __init__(self, r=0, g=0, b=0):
        self.r = r
        self.g = g
        self.b = b

def leerPPM(filename):
    try:
        with open(filename, 'r') as file:
            line = file.readline().strip()

            if line != "P3":
                print(f"Formato no soportado (debe ser P3): {line}")
                return None, None, None

            # Ignorar comentarios
            while True:
                line = file.readline()
                if not line:
                    print("Error: fin de archivo inesperado.")
                    return None, None, None
                line = line.strip()
                if line and not line.startswith('#'):
                    break

            width, height = map(int, line.split())
            max_val = int(file.readline().strip())

            data = []
            values = []
            for line in file:
                if not line.startswith('#'):
                    values.extend(line.split())

            values = list(map(int, values))
            for i in range(0, len(values), 3):
                r, g, b = values[i:i+3]
                data.append(Pixel(r, g, b))

            return width, height, data
    except FileNotFoundError:
        print(f"Error al abrir el archivo: {filename}")
        return None, None, None

def diferenciaPixel(a, b):
    return (abs(a.r - b.r) + abs(a.g - b.g) + abs(a.b - b.b)) / 3.0

def main():
    if len(sys.argv) != 3:
        print(f"Uso: {sys.argv[0]} imagen1.ppm imagen2.ppm")
        sys.exit(1)

    file1 = sys.argv[1]
    file2 = sys.argv[2]

    width1, height1, img1 = leerPPM(file1)
    width2, height2, img2 = leerPPM(file2)

    if img1 is None or img2 is None:
        sys.exit(1)

    if width1 != width2 or height1 != height2:
        print("Las imágenes tienen diferente tamaño")
        sys.exit(1)

    max_diff = 0.0
    sum_squared_diff = 0.0

    for i in range(len(img1)):
        diff = diferenciaPixel(img1[i], img2[i])
        if diff > max_diff:
            max_diff = diff
        sum_squared_diff += diff * diff

    mse = math.sqrt(sum_squared_diff / len(img1))

    print(f"Diferencia máxima: {max_diff}")
    print(f"Error cuadrático medio (MSE): {mse}")

    if max_diff < 150 and mse < 10:
        print("Resultado: aceptable")
    else:
        print("Resultado: NO aceptable")

if __name__ == "__main__":
    main()
