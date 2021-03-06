xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 42;
 84.27790;-3.33550;195.34550;,
 76.33500;-14.89600;198.75870;,
 52.07160;-22.15570;129.15910;,
 63.45500;-5.58530;124.26750;,
 65.43110;-10.66800;203.28070;,
 36.44410;-16.09440;135.63970;,
 62.47010;5.12190;204.39021;,
 32.19930;6.53590;137.22940;,
 70.41230;16.68310;200.97700;,
 43.58340;23.10630;132.33780;,
 81.31620;12.45440;196.45500;,
 59.21090;17.04570;125.85650;,
 84.27790;-3.33550;195.34550;,
 63.45500;-5.58530;124.26750;,
 25.22520;-28.91560;52.88220;,
 39.77890;-7.73150;46.62840;,
 5.24580;-21.16730;61.16810;,
 -0.17990;7.76510;63.20090;,
 14.37310;28.94990;56.94640;,
 34.35250;21.20090;48.66120;,
 39.77890;-7.73150;46.62840;,
 10.27250;-21.34440;15.66040;,
 21.13720;-5.53000;10.99210;,
 -4.64240;-15.56030;21.84700;,
 -8.69330;6.03890;23.36390;,
 2.17070;21.85400;18.69490;,
 17.08700;16.06920;12.50900;,
 21.13720;-5.53000;10.99210;,
 76.33500;-14.89600;198.75870;,
 84.27790;-3.33550;195.34550;,
 120.57640;1.66600;327.58109;,
 65.43110;-10.66800;203.28070;,
 62.47010;5.12190;204.39021;,
 70.41230;16.68310;200.97700;,
 81.31620;12.45440;196.45500;,
 21.13720;-5.53000;10.99210;,
 10.27250;-21.34440;15.66040;,
 -0.36190;-0.31290;0.36540;,
 -4.64240;-15.56030;21.84700;,
 -8.69330;6.03890;23.36390;,
 2.17070;21.85400;18.69490;,
 17.08700;16.06920;12.50900;;
 
 30;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;3,2,14,15;,
 4;2,5,16,14;,
 4;5,7,17,16;,
 4;7,9,18,17;,
 4;9,11,19,18;,
 4;11,13,20,19;,
 4;15,14,21,22;,
 4;14,16,23,21;,
 4;16,17,24,23;,
 4;17,18,25,24;,
 4;18,19,26,25;,
 4;19,20,27,26;,
 3;28,29,30;,
 3;31,28,30;,
 3;32,31,30;,
 3;33,32,30;,
 3;34,33,30;,
 3;29,34,30;,
 3;35,36,37;,
 3;36,38,37;,
 3;38,39,37;,
 3;39,40,37;,
 3;40,41,37;,
 3;41,35,37;;
 
 MeshMaterialList {
  7;
  30;
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\gsp012\\Desktop\\UFO_Triangle_v1_L2.123c33ed20b0-6dc4-4836-9410-f5bf4abee507\\13885_UFO_Triangle_diffuse - Copy.jpg";
   }
  }
  Material {
   0.204000;0.225600;0.501600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.238400;0.599200;0.156800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.533600;0.210400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.060000;0.207200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.138400;0.197600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  39;
  0.940881;-0.217889;-0.259358;,
  0.778904;-0.594287;-0.200326;,
  -0.672423;-0.651300;0.351646;,
  -0.883695;0.220393;0.412929;,
  -0.721819;0.598483;0.347557;,
  0.739211;0.649375;-0.178546;,
  0.936734;-0.218397;-0.273553;,
  0.774542;-0.595260;-0.213892;,
  -0.679421;-0.652166;0.336254;,
  -0.889918;0.220451;0.399308;,
  -0.727795;0.598719;0.334440;,
  0.734119;0.650581;-0.194458;,
  0.901427;-0.220106;-0.372803;,
  0.737818;-0.598833;-0.311486;,
  -0.727277;-0.651309;0.216482;,
  -0.929717;0.219075;0.296029;,
  -0.766345;0.596716;0.238002;,
  0.694302;0.652851;-0.302870;,
  0.661762;-0.200155;-0.722502;,
  0.523858;-0.551523;-0.649149;,
  -0.802445;-0.562141;-0.200196;,
  -0.977432;0.187123;-0.098041;,
  -0.834927;0.532576;-0.138779;,
  0.451237;0.566035;-0.689920;,
  0.943301;-0.217555;-0.250706;,
  0.111974;-0.388802;-0.914492;,
  -0.278530;-0.938161;0.205610;,
  -0.285271;-0.939317;0.190537;,
  0.347769;0.936830;-0.037490;,
  0.341960;0.938228;-0.052826;,
  -0.333795;-0.939742;0.073933;,
  0.299559;0.940963;-0.157649;,
  -0.454394;-0.832405;-0.317220;,
  0.099363;0.834968;-0.541253;,
  -0.668080;-0.650677;0.360956;,
  -0.718112;0.598276;0.355501;,
  0.351256;0.935856;-0.028171;,
  -0.916862;0.144625;-0.372084;,
  -0.068874;0.640396;-0.764951;;
  30;
  4;0,1,7,6;,
  4;26,2,8,27;,
  4;2,3,9,8;,
  4;3,4,10,9;,
  4;28,5,11,29;,
  4;5,0,6,11;,
  4;6,7,13,12;,
  4;27,8,14,30;,
  4;8,9,15,14;,
  4;9,10,16,15;,
  4;29,11,17,31;,
  4;11,6,12,17;,
  4;12,13,19,18;,
  4;30,14,20,32;,
  4;14,15,21,20;,
  4;15,16,22,21;,
  4;31,17,23,33;,
  4;17,12,18,23;,
  3;1,0,24;,
  3;2,26,34;,
  3;3,2,34;,
  3;4,3,35;,
  3;5,28,36;,
  3;0,5,24;,
  3;18,19,25;,
  3;32,20,25;,
  3;20,21,37;,
  3;21,22,37;,
  3;33,23,38;,
  3;23,18,25;;
 }
 MeshTextureCoords {
  42;
  0.375000;0.687500;,
  0.416670;0.687500;,
  0.416670;0.562190;,
  0.375000;0.562190;,
  0.458330;0.687500;,
  0.458330;0.562190;,
  0.500000;0.687500;,
  0.500000;0.562190;,
  0.541670;0.687500;,
  0.541670;0.562190;,
  0.583330;0.687500;,
  0.583330;0.562190;,
  0.625000;0.687500;,
  0.625000;0.562190;,
  0.416670;0.436870;,
  0.375000;0.436870;,
  0.458330;0.436870;,
  0.500000;0.436870;,
  0.541670;0.436870;,
  0.583330;0.436870;,
  0.625000;0.436870;,
  0.416670;0.311560;,
  0.375000;0.311560;,
  0.458330;0.311560;,
  0.500000;0.311560;,
  0.541670;0.311560;,
  0.583330;0.311560;,
  0.625000;0.311560;,
  0.421880;0.979070;,
  0.578120;0.979070;,
  0.500000;0.850000;,
  0.343750;0.843750;,
  0.421880;0.708430;,
  0.578120;0.708430;,
  0.656250;0.843750;,
  0.578120;0.020930;,
  0.421880;0.020930;,
  0.500000;0.162500;,
  0.343750;0.156250;,
  0.421880;0.291570;,
  0.578120;0.291570;,
  0.656250;0.156250;;
 }
}
