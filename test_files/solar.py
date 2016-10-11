import json
import random
import math

def rand_star_coords():
	vec = [random.gauss(0, 1) for i in range(3)]
	mag = sum(x**2 for x in vec) ** .5
	unit = [x/mag for x in vec]
	extended = [x*(random.random() * 300 + 500) for x in vec]
	return extended

def gen_star():
	return {
		"type": "sphere",
		"color": [1,1,1],
		"position": rand_star_coords(),
		"radius": (random.random() * 1) + 0.2
	}

def gen_planet(color, distance, radius, revs_per_second, steps, timeStep, revs, startTheta):

	sphere = {
		"type": "sphere",
		"radius": radius,
		"color": color,
		"frames": []
	}

	thetaStep = 2*math.pi * revs_per_second * timeStep

	for i in range(steps):
		theta = i * thetaStep + startTheta
		time = i * timeStep
		sphere["frames"].append({
			"time": time,
			"position": [math.cos(theta) * distance, 0, math.sin(theta) * distance]
		})

	return sphere

scene = []
scene.append({
	"type": "camera",
	"width": 16.0 / 13.0,
	"height": 9.0 / 13.0,
	"animated": True,
	"startTime": 0,
	"endTime": 2,
	"frameRate": 16,
	"position": [0,15,-15],
	"angle": [45,0,0]
})

#for i in range(1000):
#	scene.append(gen_star())

steps_per_rev = 32
timeStep = 0.125

#sun
scene.append({
	"type": "sphere",
	"radius": 0.75,
	"color": [237.0/255.0, 184.0/255.0, 9.0/255.0],
	"position": [0,0,0]
})

# mercury
scene.append(gen_planet(
	[150.0/255.0, 150.0/255.0, 150.0/255.0],
	2,
	0.1,
	365.26 / 87.97,
	steps_per_rev,
	timeStep,
	1,
	0
))

# venus
scene.append(gen_planet(
	[237.0/255.0, 91.0/255.0, 0.0/255.0],
	3,
	0.15,
	365.26 / 224.7,
	steps_per_rev,
	timeStep,
	1,
	0
))

# earth
scene.append(gen_planet(
	[41.0/255.0, 48.0/255.0, 150.0/255.0],
	4,
	0.15,
	1.0,
	steps_per_rev,
	timeStep,
	1,
	0
))

# mars
scene.append(gen_planet(
	[214.0/255.0, 178.0/255.0, 69.0/255.0],
	5,
	0.12,
	1.0 / 1.88,
	steps_per_rev,
	timeStep,
	1,
	0
))

# jupiter
scene.append(gen_planet(
	[214.0/255.0, 112.0/255.0, 69.0/255.0],
	7,
	0.5,
	1.0 / 11.86,
	steps_per_rev,
	timeStep,
	1,
	0
))

# saturn
scene.append(gen_planet(
	[237.0/255.0, 233.0/255.0, 123.0/255.0],
	9,
	0.35,
	1.0 / 29.46,
	steps_per_rev,
	timeStep,
	1,
	0
))

# uranus
scene.append(gen_planet(
	[67.0/255.0, 145.0/255.0, 247.0/255.0],
	11,
	0.25,
	1.0 / 84.01,
	steps_per_rev,
	timeStep,
	1,
	0
))

# neptune
scene.append(gen_planet(
	[67.0/255.0, 145.0/255.0, 247.0/255.0],
	13,
	0.25,
	1.0 / 164.79,
	steps_per_rev,
	timeStep,
	1,
	0
))

with open('solar.json', 'w') as outfile:
	json.dump(scene, outfile)