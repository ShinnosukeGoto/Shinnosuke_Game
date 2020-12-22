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
 110;
 84.66282;37.72906;52.01501;,
 -3.99565;41.30882;-71.17699;,
 35.92985;38.87766;23.41866;,
 -3.99565;41.30882;-71.17699;,
 28.46589;49.37632;23.97056;,
 -3.99565;41.30882;-71.17699;,
 22.50909;54.20068;24.44451;,
 -3.99565;41.30882;-71.17699;,
 14.74603;57.90258;24.80815;,
 -3.99565;41.30882;-71.17699;,
 5.70579;60.22967;25.03676;,
 -3.99565;41.30882;-71.17699;,
 -3.99560;61.02342;25.11475;,
 -3.99565;41.30882;-71.17699;,
 -13.69699;60.22967;25.03676;,
 -3.99565;41.30882;-71.17699;,
 -22.73724;57.90258;24.80815;,
 -3.99565;41.30882;-71.17699;,
 -30.50029;54.20068;24.44451;,
 -3.99565;41.30882;-71.17699;,
 -36.45708;49.37632;23.97062;,
 -3.99565;41.30882;-71.17699;,
 -43.56792;38.83292;23.41866;,
 -3.99565;41.30882;-71.17699;,
 -89.72704;37.72906;48.78392;,
 -3.99565;41.30882;-71.17699;,
 -45.81989;25.77034;22.23418;,
 -3.99565;41.30882;-71.17699;,
 -36.45708;19.12755;21.61796;,
 -3.99565;41.30882;-71.17699;,
 -29.34027;13.87199;21.20838;,
 -3.99565;41.30882;-71.17699;,
 -22.54813;9.26509;20.93799;,
 -3.99565;41.30882;-71.17699;,
 -13.69699;4.64241;20.61609;,
 -3.99565;41.30882;-71.17699;,
 -3.99560;-1.56076;20.53815;,
 -3.99565;41.30882;-71.17699;,
 5.70579;4.09762;20.61609;,
 -3.99565;41.30882;-71.17699;,
 12.40776;8.37770;20.84472;,
 -3.99565;41.30882;-71.17699;,
 19.25896;13.64667;21.20838;,
 -3.99565;41.30882;-71.17699;,
 27.15512;19.76141;21.68230;,
 -3.99565;41.30882;-71.17699;,
 36.28536;25.20126;21.92042;,
 -3.99565;41.30882;-71.17699;,
 84.66282;37.72906;52.01501;,
 -4.27121;21.07888;42.93140;,
 84.66282;37.72906;52.01501;,
 35.92985;38.87766;23.41866;,
 28.46589;49.37632;23.97056;,
 22.50909;54.20068;24.44451;,
 14.74603;57.90258;24.80815;,
 5.70579;60.22967;25.03676;,
 -3.99560;61.02342;25.11475;,
 -13.69699;60.22967;25.03676;,
 -22.73724;57.90258;24.80815;,
 -30.50029;54.20068;24.44451;,
 -36.45708;49.37632;23.97062;,
 -43.56792;38.83292;23.41866;,
 -89.72704;37.72906;48.78392;,
 -45.81989;25.77034;22.23418;,
 -36.45708;19.12755;21.61796;,
 -29.34027;13.87199;21.20838;,
 -22.54813;9.26509;20.93799;,
 -13.69699;4.64241;20.61609;,
 -3.99560;-1.56076;20.53815;,
 5.70579;4.09762;20.61609;,
 12.40776;8.37770;20.84472;,
 19.25896;13.64667;21.20838;,
 27.15512;19.76141;21.68230;,
 36.28536;25.20126;21.92042;,
 35.73198;38.77715;23.82050;,
 -1.45371;78.60277;51.35573;,
 -1.45371;49.21458;76.68111;,
 -1.45371;78.60277;51.35573;,
 -43.27893;38.63480;23.85906;,
 -1.45371;78.60277;51.35573;,
 -1.45371;44.48911;6.55792;,
 -1.45371;78.60277;51.35573;,
 35.73198;38.77715;23.82050;,
 -4.17011;20.44276;45.03852;,
 35.73198;38.77715;23.82050;,
 -1.45371;49.21458;76.68111;,
 -43.27893;38.63480;23.85906;,
 -1.45371;44.48911;6.55792;,
 -1.19914;78.25668;51.54850;,
 74.55418;96.66800;32.31261;,
 35.70678;38.63628;23.30550;,
 74.55418;96.66800;32.31261;,
 -1.71020;49.01754;77.37203;,
 74.55418;96.66800;32.31261;,
 -1.19914;78.25668;51.54850;,
 7.22174;60.84340;50.39560;,
 -1.19914;78.25668;51.54850;,
 35.70678;38.63628;23.30550;,
 -1.71020;49.01754;77.37203;,
 -1.51775;78.56723;51.11955;,
 -78.53735;102.63794;30.89233;,
 -1.40638;49.17678;76.39194;,
 -78.53735;102.63794;30.89233;,
 -43.34737;38.65114;23.29660;,
 -78.53735;102.63794;30.89233;,
 -1.51775;78.56723;51.11955;,
 -10.82815;56.04560;52.56007;,
 -1.51775;78.56723;51.11955;,
 -1.40638;49.17678;76.39194;,
 -43.34737;38.65114;23.29660;;
 
 68;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;6,7,8;,
 3;8,9,10;,
 3;10,11,12;,
 3;12,13,14;,
 3;14,15,16;,
 3;16,17,18;,
 3;18,19,20;,
 3;20,21,22;,
 3;22,23,24;,
 3;24,25,26;,
 3;26,27,28;,
 3;28,29,30;,
 3;30,31,32;,
 3;32,33,34;,
 3;34,35,36;,
 3;36,37,38;,
 3;38,39,40;,
 3;40,41,42;,
 3;42,43,44;,
 3;44,45,46;,
 3;46,47,48;,
 3;49,50,51;,
 3;49,51,52;,
 3;49,52,53;,
 3;49,53,54;,
 3;49,54,55;,
 3;49,55,56;,
 3;49,56,57;,
 3;49,57,58;,
 3;49,58,59;,
 3;49,59,60;,
 3;49,60,61;,
 3;49,61,62;,
 3;49,62,63;,
 3;49,63,64;,
 3;49,64,65;,
 3;49,65,66;,
 3;49,66,67;,
 3;49,67,68;,
 3;49,68,69;,
 3;49,69,70;,
 3;49,70,71;,
 3;49,71,72;,
 3;49,72,73;,
 3;49,73,50;,
 3;74,75,76;,
 3;76,77,78;,
 3;78,79,80;,
 3;80,81,82;,
 3;83,84,85;,
 3;83,85,86;,
 3;83,86,87;,
 3;83,87,84;,
 3;88,89,90;,
 3;90,91,92;,
 3;92,93,94;,
 3;95,96,97;,
 3;95,97,98;,
 3;95,98,96;,
 3;99,100,101;,
 3;101,102,103;,
 3;103,104,105;,
 3;106,107,108;,
 3;106,108,109;,
 3;106,109,107;;
 
 MeshMaterialList {
  2;
  68;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  1,
  1,
  1,
  0,
  0,
  0,
  1,
  1,
  1;;
  Material {
   0.800000;0.119200;0.357600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  76;
  -0.100737;0.972341;0.210727;,
  0.225965;0.973304;0.040247;,
  0.686565;0.666408;-0.290740;,
  0.504854;0.825822;-0.251276;,
  0.325612;0.918963;-0.222447;,
  0.159035;0.965560;-0.205917;,
  -0.000000;0.979678;-0.200577;,
  -0.159035;0.965560;-0.205917;,
  -0.325612;0.918964;-0.222447;,
  -0.504855;0.825822;-0.251276;,
  -0.694121;0.657718;-0.292580;,
  -0.235688;0.970757;0.045617;,
  0.097083;0.970900;0.218923;,
  -0.440585;-0.897249;0.028785;,
  -0.559116;-0.740634;-0.372627;,
  -0.548148;-0.748733;-0.372737;,
  -0.486369;-0.790210;-0.372845;,
  -0.472114;-0.800323;-0.369582;,
  -0.019409;-0.905750;-0.423367;,
  0.489468;-0.788910;-0.371540;,
  0.544998;-0.753881;-0.366935;,
  0.581811;-0.727226;-0.364196;,
  0.551189;-0.833622;-0.035556;,
  0.406215;-0.913539;0.020889;,
  -0.001315;0.973608;-0.228223;,
  -0.036426;0.470491;0.881653;,
  0.783308;0.405804;0.470905;,
  -0.746477;0.434382;0.504068;,
  0.018092;0.795456;-0.605742;,
  0.569764;-0.631789;0.525558;,
  -0.709517;-0.135263;-0.691584;,
  -0.694498;-0.163727;-0.700618;,
  0.651431;-0.521596;0.550977;,
  -0.308822;0.344340;-0.886600;,
  -0.791804;-0.384492;-0.474565;,
  0.004640;0.651992;0.758212;,
  -0.661959;-0.436390;0.609404;,
  0.755492;-0.428015;-0.496018;,
  -0.317038;-0.943182;-0.099473;,
  0.002863;-0.907299;-0.420476;,
  0.307866;-0.946045;-0.101079;,
  0.396079;-0.619130;0.678085;,
  0.353291;0.203165;0.913186;,
  0.324090;0.238481;0.915474;,
  0.250953;0.309097;0.917323;,
  0.164948;0.365646;0.916021;,
  0.081692;0.397020;0.914167;,
  -0.000018;0.407351;0.913272;,
  -0.081878;0.397936;0.913752;,
  -0.165597;0.367388;0.915207;,
  -0.252418;0.311445;0.916126;,
  -0.330065;0.236848;0.913761;,
  -0.363085;0.197205;0.910648;,
  -0.367069;-0.717141;0.592427;,
  -0.406968;-0.619476;0.671287;,
  -0.405259;-0.618773;0.672968;,
  -0.367172;-0.657260;0.658175;,
  -0.371182;-0.665839;0.647211;,
  -0.018104;-0.703223;0.710738;,
  0.380593;-0.645967;0.661721;,
  0.408379;-0.623332;0.666846;,
  0.428697;-0.604688;0.671246;,
  0.428357;-0.605159;0.671039;,
  -0.340356;0.748087;-0.569670;,
  0.374151;0.737801;-0.561837;,
  -0.579860;-0.577392;0.574788;,
  0.004652;-0.848177;-0.529693;,
  -0.102586;-0.840293;-0.532337;,
  0.111836;-0.846247;-0.520921;,
  0.029562;0.661392;0.749458;,
  -0.799125;-0.406576;-0.442825;,
  0.571925;-0.027891;-0.819831;,
  0.326082;0.286281;-0.900951;,
  0.687066;-0.153812;-0.710128;,
  0.765955;-0.470273;-0.438357;,
  0.697800;-0.565996;-0.439002;;
  68;
  3;0,24,1;,
  3;1,24,2;,
  3;2,24,3;,
  3;3,24,4;,
  3;4,24,5;,
  3;5,24,6;,
  3;6,24,7;,
  3;7,24,8;,
  3;8,24,9;,
  3;9,24,10;,
  3;10,24,11;,
  3;11,24,12;,
  3;38,39,13;,
  3;13,39,14;,
  3;14,39,15;,
  3;15,39,16;,
  3;16,39,17;,
  3;17,39,18;,
  3;18,39,19;,
  3;19,39,20;,
  3;20,39,21;,
  3;21,39,22;,
  3;22,39,23;,
  3;23,39,40;,
  3;25,0,1;,
  3;41,42,43;,
  3;25,43,44;,
  3;25,44,45;,
  3;25,45,46;,
  3;25,46,47;,
  3;25,47,48;,
  3;25,48,49;,
  3;25,49,50;,
  3;25,50,51;,
  3;25,51,52;,
  3;25,11,12;,
  3;53,38,13;,
  3;53,13,54;,
  3;53,54,55;,
  3;53,55,56;,
  3;53,56,57;,
  3;53,57,58;,
  3;41,58,59;,
  3;41,59,60;,
  3;41,60,61;,
  3;41,61,62;,
  3;41,22,23;,
  3;41,23,40;,
  3;26,26,26;,
  3;27,27,27;,
  3;63,28,28;,
  3;28,28,64;,
  3;29,29,29;,
  3;65,65,65;,
  3;66,67,66;,
  3;66,66,68;,
  3;30,33,31;,
  3;32,32,32;,
  3;69,69,69;,
  3;34,30,31;,
  3;34,31,70;,
  3;34,70,30;,
  3;35,35,35;,
  3;36,36,36;,
  3;71,72,73;,
  3;37,73,74;,
  3;37,74,75;,
  3;37,71,73;;
 }
 MeshTextureCoords {
  110;
  0.000000;1.000000;,
  0.020830;0.000000;,
  0.041670;1.000000;,
  0.041670;0.000000;,
  0.083330;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.083330;0.000000;,
  0.166670;1.000000;,
  0.104170;0.000000;,
  0.208330;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.145830;0.000000;,
  0.291670;1.000000;,
  0.166670;0.000000;,
  0.333330;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.208330;0.000000;,
  0.416670;1.000000;,
  0.229170;0.000000;,
  0.458330;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.270830;0.000000;,
  0.541670;1.000000;,
  0.291670;0.000000;,
  0.583330;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.333330;0.000000;,
  0.666670;1.000000;,
  0.354170;0.000000;,
  0.708330;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.395830;0.000000;,
  0.791670;1.000000;,
  0.416670;0.000000;,
  0.833330;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.458330;0.000000;,
  0.916670;1.000000;,
  0.479170;0.000000;,
  0.958330;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.166670;0.000000;,
  0.333330;1.000000;,
  0.333330;0.000000;,
  0.666670;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.166670;0.000000;,
  0.333330;1.000000;,
  0.333330;0.000000;,
  0.666670;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
