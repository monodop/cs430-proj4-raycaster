python scene_gen.py
../bin/raycast 4096 2160 rand_scene.json out.ppm
convert out.ppm out.png
rm out.ppm