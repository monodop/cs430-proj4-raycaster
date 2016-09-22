import json
import random

def rand_coords():
	x = (random.random() - 0.5) * 40
	y = (random.random() - 0.5) * 25
	z = random.random() * 10 + 30
	return [x,y,z]

def gen_sphere():
	return {
		"type": "sphere",
		"color": [random.random(), random.random(), random.random()],
		"position": rand_coords(),
		"radius": (random.random() * 0.5) + 0.1
	}

scene = []
scene.append({
	"type": "camera",
	"width": 16.0 / 13.0,
	"height": 9.0 / 13.0
	})

for i in range(20000):
	scene.append(gen_sphere())

with open('rand_scene.json', 'w') as outfile:
	json.dump(scene, outfile)