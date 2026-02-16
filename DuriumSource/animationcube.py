import tkinter as tk
import math
import random

class MatrixCubeApp:
    def __init__(self, root):
        self.root = root
        self.root.title("LEAVE ME ALONE")
        self.canvas = tk.Canvas(root, width=800, height=600, bg='black', highlightthickness=0)
        self.canvas.pack()

        # Configurações Matrix
        self.width = 800
        self.height = 600
        self.columns = self.width // 20
        self.drops = [0] * self.columns
        
        # Configurações Cubo
        self.angle = 0
        self.points = [
            [-1, -1,  1], [1, -1,  1], [1,  1,  1], [-1,  1,  1],
            [-1, -1, -1], [1, -1, -1], [1,  1, -1], [-1,  1, -1]
        ]
        # Conexões entre os pontos para formar as linhas do cubo
        self.edges = [
            (0,1), (1,2), (2,3), (3,0),
            (4,5), (5,6), (6,7), (7,4),
            (0,4), (1,5), (2,6), (3,7)
        ]

        self.animation_step()

    def rotate(self, x, y, z):
        # Rotação em múltiplos eixos
        rad = self.angle
        # Rotação Y
        nx = x * math.cos(rad) + z * math.sin(rad)
        nz = -x * math.sin(rad) + z * math.cos(rad)
        # Rotação X
        ny = y * math.cos(rad) - nz * math.sin(rad)
        nz = y * math.sin(rad) + nz * math.cos(rad)
        return nx, ny, nz

    def draw_matrix(self):
        # Limpa levemente para efeito de rastro (simulado no Tkinter redesenhando)
        # Nota: O Tkinter não lida bem com transparência alpha no canvas, 
        # então vamos apenas limpar e redesenhar os caracteres.
        for i in range(len(self.drops)):
            char = chr(random.randint(33, 126))
            # Cor RGB aleatória
            color = f'#{random.randint(0,255):02x}{random.randint(0,255):02x}{random.randint(0,255):02x}'
            
            x = i * 20
            y = self.drops[i] * 20
            
            # Criamos o texto (tag 'matrix' para podermos limpar depois)
            self.canvas.create_text(x, y, text=char, fill=color, font=("Courier", 12), tags="matrix")

            if self.drops[i] * 20 > self.height or random.random() > 0.95:
                self.drops[i] = 0
            self.drops[i] += 1

    def draw_cube(self):
        projected = []
        for p in self.points:
            rx, ry, rz = self.rotate(p[0], p[1], p[2])
            # Projeção perspectiva simples
            factor = 300 / (rz + 4)
            x = rx * factor + self.width // 2
            y = ry * factor + self.height // 2
            projected.append((x, y))

        for edge in self.edges:
            p1 = projected[edge[0]]
            p2 = projected[edge[1]]
            # Cor RGB pulsante para o cubo
            c = f'#{int(127+127*math.sin(self.angle)):02x}ff{int(127+127*math.cos(self.angle)):02x}'
            self.canvas.create_line(p1[0], p1[1], p2[0], p2[1], fill=c, width=3, tags="cube")

    def animation_step(self):
        # Limpa o canvas
        self.canvas.delete("matrix")
        self.canvas.delete("cube")
        
        self.draw_matrix()
        self.draw_cube()
        
        self.angle += 0.05
        # 50ms de delay (aprox 20 FPS para manter o Tkinter fluido)
        self.root.after(50, self.animation_step)

if __name__ == "__main__":
    root = tk.Tk()
    app = MatrixCubeApp(root)
    root.mainloop()
