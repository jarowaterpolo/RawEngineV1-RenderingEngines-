#version 330 core
//region vars
out vec4 FragColor;
in vec3 fPos;
in vec2 fUV;

uniform vec4 paintColor;
uniform float blueIntensity;
uniform float time;

vec4 red = vec4(1,0,0,1);
vec4 black = vec4(0,0,0,1);
vec4 white = vec4(1,1,1,1);
vec4 grey = vec4(.5,.5,.5,1);
vec4 yellow = vec4(.8,.8,0,1);
vec4 brown = vec4(0.59,0.29,0,1);

vec4 gridA2ColYel = vec4(.94,.82,.47,1);
vec4 gridA2ColBrn = vec4(.31,.27,.16,1);
//endregion

void CheckerBoard(){
   float gridSize = 4;
   vec2 grid = floor(fUV * gridSize);

   if (mod(grid.x + grid.y, 2.0) == 0.0){
      FragColor = red;
   }
   else{
      FragColor = black;
   }
}

void Donut(){
   float powerMult = 2.0;
   vec2 offset = vec2(0.5, 0.5);
   float x = fUV.x - offset.x;
   float y = fUV.y - offset.y;
   float r = sqrt(pow(x, powerMult) + pow(y, powerMult));

   float c1 = length(r*1.5);
   float dist = pow(c1, 2);

   if (dist < .4)
   {
      FragColor = red * dist;
   }
   else
   {
      FragColor = black;
   }
}

void VerticalSmallLines(){
   float lineAmount = 12;
   float x = floor(fUV.x * lineAmount);

   if (mod(x, 2.0) == 0.0){
      if (fract(fUV.x * lineAmount) < .6){
         FragColor = red;
      }
   }
   else{
      FragColor = black;
   }
}

void CheckerBoardYellow(){
   float gridSize = 4;
   vec2 grid = floor(fUV * gridSize);
   float diagonal = 1.5 - (grid.x * .2 + grid.y * .4) * .8;


   if (mod(grid.x + grid.y, 2.0) == 0.0){
      FragColor = clamp(mix(gridA2ColYel, gridA2ColBrn, sin(time) * diagonal), gridA2ColYel, black);
   }
   else{
      FragColor = black;
   }
}

void DonutGreyWithWhite(){
   float powerMult = 2.0;
   vec2 offset = vec2(0.5, 0.5);
   float x = fUV.x - offset.x;
   float y = fUV.y - offset.y;
   float r = sqrt(pow(x, powerMult) + pow(y, powerMult));

   float c1 = length(r*1.5);
   float dist = pow(c1, 2);

   if (dist < .4)
   {
//      FragColor = mix(white, black, (dist * 4) + .1) * grey + (1-grey);
//         FragColor = white - grey - dist;
         FragColor = grey + grey / 2.5 - dist;
//      FragColor = clamp( white
//                         - dist * grey * 4
//      , grey, white);
   }
   else
   {
      FragColor = black;
   }
}

void DiagonalSmallLines(){
   float lineAmount = 6;
   float difference = fUV.x * lineAmount - fUV.y * lineAmount;
   float colDifference = (fUV.x - fUV.y);
//   vec4 RedToGreen = vec4(floor(fUV.y), ceil(fUV.y), 0, 1);
   vec4 RedToGreen = vec4(fUV.y / 3, fUV.x, 0, 1);
   vec4 BlueToPink = vec4((fUV.x - fUV.y), 0, 1, 1);
   if (mod(floor(difference), 2.0) == 0.0){
      if (fract(difference) < .8 && fract(difference) > .2){
         FragColor = RedToGreen;
      }
      else{
         FragColor = black;
      }

   }
   else{
      if (fract(difference) < .8 && fract(difference) > .2){
         FragColor = BlueToPink;
//         FragColor = red;
      }
      else{
         FragColor = black;
      }
   }
}

void main()
{
//   FragColor = vec4(fPos.x + paintColor.x, fPos.y + paintColor.y, fPos.z + paintColor.z, 1);
//   FragColor = vec4(fUV.x, fUV.y, blueIntensity, 1);

//   CheckerBoard();
//   Donut();
//   VerticalSmallLines();
//   CheckerBoardYellow();
//   DonutGreyWithWhite();
   DiagonalSmallLines();
}

