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
 20;
 4.42598;-0.88993;-4.68901;,
 -4.42882;-0.88993;-4.68901;,
 -3.71712;-0.09003;-3.89341;,
 3.71418;-0.09003;-3.89341;,
 -3.71712;-0.09003;4.41309;,
 3.71418;-0.09003;4.41309;,
 -4.42882;-0.88993;5.20859;,
 4.42598;-0.88993;5.20859;,
 3.89388;-10.45233;4.61389;,
 -3.89682;-10.45233;4.61389;,
 -3.89682;-10.45233;-4.09421;,
 3.89388;-10.45233;-4.09421;,
 -4.42882;-0.88993;5.20859;,
 -3.71712;-0.09003;4.41309;,
 4.42598;-0.88993;5.20859;,
 3.71418;-0.09003;4.41309;,
 3.89388;-10.45233;-4.09421;,
 -3.89682;-10.45233;-4.09421;,
 -3.89682;-10.45233;4.61389;,
 3.89388;-10.45233;4.61389;;
 
 10;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;8,9,10,11;,
 4;1,12,13,2;,
 4;14,0,3,15;,
 4;16,17,1,0;,
 4;17,18,12,1;,
 4;7,6,9,8;,
 4;19,16,0,14;;
 
 MeshMaterialList {
  5;
  10;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.048000;0.048000;0.072000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.032000;0.120000;0.568000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.568000;0.424000;0.032000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.568000;0.424000;0.032000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.568000;0.424000;0.032000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  13;
  0.000000;-1.000000;0.000000;,
  0.289095;0.917138;-0.274375;,
  -0.289117;0.917130;-0.274379;,
  0.289099;0.917131;0.274395;,
  -0.289120;0.917123;0.274399;,
  0.000000;0.352546;-0.935794;,
  0.000000;0.352522;0.935804;,
  -0.944157;0.329496;0.000000;,
  0.944147;0.329523;0.000000;,
  0.000000;-0.062082;-0.998071;,
  -0.998456;-0.055549;0.000000;,
  0.000000;-0.062072;0.998072;,
  0.998455;-0.055559;-0.000000;;
  10;
  4;5,5,2,1;,
  4;1,2,4,3;,
  4;3,4,6,6;,
  4;0,0,0,0;,
  4;7,7,4,2;,
  4;8,8,1,3;,
  4;9,9,5,5;,
  4;10,10,7,7;,
  4;6,6,11,11;,
  4;12,12,8,8;;
 }
 MeshTextureCoords {
  20;
  0.375000;0.230700;,
  0.625000;0.230700;,
  0.625000;0.250000;,
  0.375000;0.250000;,
  0.625000;0.500000;,
  0.375000;0.500000;,
  0.625000;0.519300;,
  0.375000;0.519300;,
  0.375000;0.750000;,
  0.625000;0.750000;,
  0.625000;1.000000;,
  0.375000;1.000000;,
  0.875000;0.230700;,
  0.875000;0.250000;,
  0.125000;0.230700;,
  0.125000;0.250000;,
  0.375000;0.000000;,
  0.625000;0.000000;,
  0.875000;0.000000;,
  0.125000;0.000000;;
 }
}
