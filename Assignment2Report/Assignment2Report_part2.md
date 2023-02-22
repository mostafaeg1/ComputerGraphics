# Assignment2 Report - Part 2:
## 1.Ambient Lighting
First, I started making a new class for the light source. I added 3 RGB values For each one of diffuse,ambient and specular lights.
I also added 3 RGB values to the mesh model as these represent the material of the object.\
I used the formula that we saw at the lecture to calculate the ambient light:
```
auto la = scene.GetActiveLight().ambient;
auto ka = model.getAmbient();
auto Ia = glm::vec3(la.x * ka.x, la.y * ka.y, la.z * ka.z);
```
### First Example
Illuminating with a white ambient light, on a blue ambient teapot, resulted in a blue teapot.
As discussed in class, the ambient lighting is computed by multiplying the material's ambient with the light's ambient.
Since the ambient of the teapot is blue and the ambient of the light is white, which is a 3d vector of 1's, the ambient lighting is similar to the ambient component of the teapot.
![image info](./images/Assignment2_part2_teapot_blue.png)
On the other hand, illumanting with a red ambient light on the same teapot, resulted in a black teapot.
The light's ambient red coordinate is the only non-zero coordinate, while the teapot's ambient red coordinate is the only zero coordinate.
Multiplying these two vectors, results in a 3d vector of zeroes, which is black in color.
![image info](./images/Assignment2_part2_teapot_black.png)
![image info](./images/Assignment2_part2_teapot_demo.gif)
### Second Example
As can be seen in the screenshot below, I changed the ambient lighting of a banana to a yellow shade, which has a non-zero value for the red, green, and blue coordinates.
![image info](./images/Assignment2_part2_banana_yellow.png)
Since all of the banana's ambient coordinates have a non-zero value, illuminating it with a red ambient light resulted in a red shaded banana.
![image info](./images/Assignment2_part2_banana_red.png)
Illuminating with a green ambient light, resulted in a green shaded banana.
![image info](./images/Assignment2_part2_banana_green.png)
And illuminating with a blue ambient light resulted in a blue shaded banana.
![image info](./images/Assignment2_part2_banana_blue.png)
## 2.Diffuse Lighting
I calculated the diffuse light according to the formula we saw at the lecture:
```
auto ld = scene.GetActiveLight().diffuse
auto kd = model.getDiffuse()
auto light_position = scene.GetActiveLight().GetPosition()
auto angle= getAngle(normal, light_position)
auto Id= glm::vec3(angle*kd.x * ld.x,angle* kd.y * ld.y, angle*kd.z * ld.z)
```
Following are screenshots of the scene displayed in the "Ambient Lighting" examples, with a white ambient and diffuse light.
![image info](./images/Assignment2_part2_teapot_diffuse1.png)
![image info](./images/Assignment2_part2_teapot_diffuse2.png)
![image info](./images/Assignment2_part2_banana_diffuse1.png)
![image info](./images/Assignment2_part2_banana_diffuse2.png)
![image info](./images/Assignment2_part2_banana_demo.gif)
#### Tea-pot with red ambient,diffuse material and white  ambient,diffuse lighting.
![image info](./images/Assignment2_part2_teapot_red.png)

## 4.Reflection Vector
for each face I calculated the reflected vector using this formula:
```
r = 2 * n * glm::dot(n, l) - l
```
and then  draw it in cyan , for light ray direction I used the l vector direction light (lightlocation-point) then draw it in green.
Below are screenshots of the direction of light and reflected light for different light positions.
### Light Position 0x0
![image info](./images/Assignment2_part2_light00.gif)
### Light Position 0x1000
![image info](./images/Assignment2_part2_light01.gif)
### Light Position 1000x1000
![image info](./images/Assignment2_part2_light11.gif)
## 5.PhongShading with specular light
I calculated the specular light according to the formula we saw in the lecture:
```
r = 2 * n * glm::dot(n, l) - l
v = current_model.getCameraEyeVector()
Is= Ks * glm::dot(r,v) * ls
```
The screenshots below illustrate phong shading with and without shininess for specular lighting.
![image info](./images/teapotnew.png)
![image info](./images/teapotnew2.png)
![image info](./images/teapotnew4.png)
![image info](./images/teapotnew6.png)
## 6.Demonstrations
### Rabbit 
![image info](./images/rabbit5.png)
![image info](./images/rabbit11.png)
![image info](./images/rabbit9.png)
![image info](./images/rabbit8.png)
### Cow
#### flat shading
![image info](./images/Assignment2_part2_cow_brown_flatshading.png)
#### phong shading
![image info](./images/cow5.png)
![image info](./images/cow14.png)
![image info](./images/cow7.png)
![image info](./images/cow12.png)
![image info](./images/cow10.png)
![image info](./images/cow11.png)
![image info](./images/cow2.png)
### Chain
![image info](./images/Assignment2_part2_chain_spec_red.png)
### Banana
![image info](./images/banana9.png)
![image info](./images/banana8.png)
### Beethoven
#### flat shading
![image info](./images/Assignment2_part2_bethoven_yellow.png)
#### phong shading
![image info](./images/bithovengray.png)
![image info](./images/bethoven.png)










