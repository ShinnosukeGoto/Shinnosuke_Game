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
 247;
 0.00000;11.60158;-25.88820;,
 -18.84730;10.81178;-22.70470;,
 -17.95130;32.37178;-20.65660;,
 0.00000;34.76707;-23.71210;,
 -34.57450;10.02208;-13.21760;,
 -32.94250;29.97647;-11.58550;,
 -28.00910;50.39328;-6.65210;,
 -16.61450;59.23298;-11.58550;,
 0.00000;63.59578;-14.02030;,
 34.57450;10.02208;-13.21760;,
 18.84730;10.81188;-22.70470;,
 17.95130;32.37178;-20.65660;,
 32.94250;29.97647;-11.58550;,
 16.61450;59.23298;-11.58550;,
 28.00910;50.39328;-6.65210;,
 17.95130;75.48647;3.40580;,
 32.94250;59.23298;4.74260;,
 0.00000;80.96139;2.06900;,
 0.00000;86.77848;21.35700;,
 19.28810;80.96139;21.35700;,
 35.37740;63.59578;21.35700;,
 -17.95130;75.48647;3.40580;,
 -32.94250;59.23298;4.74260;,
 -35.37740;63.59578;21.35700;,
 -19.28810;80.96139;21.35700;,
 -17.95130;75.48647;39.30830;,
 0.00000;80.96139;40.64510;,
 -32.94250;59.23298;37.97150;,
 -28.00910;50.39328;49.36610;,
 -16.61450;59.23298;54.29960;,
 0.00000;63.59578;56.73440;,
 17.95130;75.48647;39.30830;,
 32.94250;59.23298;37.97150;,
 16.61450;59.23298;54.29960;,
 28.00910;50.39328;49.36610;,
 17.95130;32.37178;63.37070;,
 32.94250;29.97647;54.29960;,
 0.00000;34.76707;66.42620;,
 0.00000;11.60158;68.60230;,
 18.84730;10.81178;65.41870;,
 34.57450;10.02208;55.93160;,
 -17.95130;32.37178;63.37070;,
 -32.94250;29.97647;54.29960;,
 -34.57450;10.02208;55.93160;,
 -18.84730;10.81178;65.41870;,
 -47.24530;11.60158;21.35700;,
 -44.06170;10.81188;40.20440;,
 -42.01360;32.37178;39.30830;,
 -45.06920;34.76707;21.35700;,
 -34.57450;10.02208;55.93160;,
 -32.94250;29.97647;54.29960;,
 -28.00910;50.39328;49.36610;,
 -32.94250;59.23298;37.97150;,
 -35.37740;63.59578;21.35700;,
 -44.06170;10.81188;2.50970;,
 -42.01360;32.37178;3.40580;,
 -32.94250;59.23298;4.74260;,
 47.24530;11.60158;21.35700;,
 44.06170;10.81178;2.50970;,
 42.01360;32.37178;3.40580;,
 45.06920;34.76707;21.35700;,
 32.94250;59.23298;4.74260;,
 35.37740;63.59578;21.35700;,
 34.57450;10.02208;55.93160;,
 44.06170;10.81188;40.20440;,
 42.01360;32.37178;39.30830;,
 32.94250;29.97647;54.29960;,
 32.94250;59.23298;37.97150;,
 28.00910;50.39328;49.36610;,
 48.31570;0.20678;21.35700;,
 45.06920;0.20678;40.64510;,
 45.06920;0.20678;2.06900;,
 35.37740;0.20678;56.73440;,
 35.37740;0.20678;-14.02030;,
 19.28810;0.20678;66.42620;,
 19.28810;0.20678;-23.71210;,
 0.00000;0.20678;69.67260;,
 0.00000;0.20678;-26.95870;,
 -19.28810;0.20678;66.42620;,
 -19.28810;0.20678;-23.71210;,
 -35.37740;0.20678;56.73440;,
 -35.37740;0.20678;-14.02030;,
 -45.06920;0.20678;40.64510;,
 -45.06920;0.20678;2.06900;,
 -48.31570;0.20678;21.35700;,
 38.63780;-0.31833;-17.28080;,
 21.06570;-0.31833;-27.86580;,
 20.58430;11.26418;-26.76550;,
 37.76100;10.40158;-16.40400;,
 0.00000;-0.31833;-31.41150;,
 0.00000;12.12668;-30.24240;,
 -21.06570;-0.31833;-27.86580;,
 -20.58430;11.26418;-26.76550;,
 -38.63780;-0.31833;-17.28080;,
 -37.76100;10.40158;-16.40400;,
 -49.22280;-0.31833;0.29130;,
 -48.12250;11.26418;0.77270;,
 -52.76850;-0.31833;21.35700;,
 -51.59950;12.12668;21.35700;,
 -49.22280;-0.31833;42.42270;,
 -48.12250;11.26418;41.94130;,
 -38.63780;-0.31833;59.99480;,
 -37.76100;10.40158;59.11810;,
 -20.58430;11.26418;69.47950;,
 -37.76100;10.40158;59.11810;,
 -38.63780;-0.31833;59.99480;,
 -21.06570;-0.31833;70.57980;,
 0.00000;12.12668;72.95650;,
 0.00000;-0.31833;74.12560;,
 20.58430;11.26418;69.47950;,
 21.06570;-0.31833;70.57980;,
 37.76100;10.40158;59.11810;,
 38.63780;-0.31833;59.99480;,
 38.63780;-0.31833;59.99480;,
 49.22280;-0.31833;42.42270;,
 48.12250;11.26418;41.94130;,
 37.76100;10.40158;59.11810;,
 52.76850;-0.31833;21.35700;,
 51.59950;12.12668;21.35700;,
 49.22280;-0.31833;0.29130;,
 48.12250;11.26418;0.77270;,
 -35.37740;0.20678;56.73440;,
 35.37740;0.20678;56.73440;,
 -11.69910;52.99038;2.38450;,
 -9.95190;56.41957;2.38450;,
 -9.95190;56.41957;-75.89050;,
 -11.69910;52.99038;-75.89050;,
 -7.23050;59.14098;2.38450;,
 -7.23050;59.14098;-75.89050;,
 -3.80130;60.88818;2.38450;,
 -3.80130;60.88818;-75.89050;,
 0.00000;61.49028;2.38450;,
 0.00000;61.49028;-75.89050;,
 3.80130;60.88818;2.38450;,
 3.80130;60.88818;-75.89050;,
 7.23040;59.14098;2.38450;,
 7.23040;59.14098;-75.89050;,
 9.95190;56.41957;2.38450;,
 9.95190;56.41957;-75.89050;,
 11.69910;52.99038;2.38450;,
 11.69910;52.99038;-75.89050;,
 12.30120;49.18908;2.38450;,
 12.30120;49.18908;-75.89050;,
 11.69910;45.38778;2.38450;,
 11.69910;45.38778;-75.89050;,
 9.95190;41.95868;2.38450;,
 9.95190;41.95868;-75.89050;,
 7.23040;39.23717;2.38450;,
 7.23040;39.23717;-75.89050;,
 3.80130;37.48997;2.38450;,
 3.80130;37.48997;-75.89050;,
 0.00000;36.88788;2.38450;,
 0.00000;36.88788;-75.89050;,
 -3.80130;37.48997;2.38450;,
 -3.80130;37.48997;-75.89050;,
 -7.23040;39.23717;2.38450;,
 -7.23040;39.23717;-75.89050;,
 -9.95190;41.95868;2.38450;,
 -9.95190;41.95868;-75.89050;,
 -11.69910;45.38778;2.38450;,
 -11.69910;45.38778;-75.89050;,
 -12.30120;49.18908;2.38450;,
 -12.30120;49.18908;-75.89050;,
 -11.69910;52.99038;2.38450;,
 -11.69910;52.99038;-75.89050;,
 -9.95190;56.41957;2.38450;,
 -11.69910;52.99038;2.38450;,
 0.00000;49.18908;2.38450;,
 -7.23050;59.14098;2.38450;,
 -3.80130;60.88818;2.38450;,
 0.00000;61.49028;2.38450;,
 3.80130;60.88818;2.38450;,
 7.23040;59.14098;2.38450;,
 9.95190;56.41957;2.38450;,
 11.69910;52.99038;2.38450;,
 12.30120;49.18908;2.38450;,
 11.69910;45.38778;2.38450;,
 9.95190;41.95868;2.38450;,
 7.23040;39.23717;2.38450;,
 3.80130;37.48997;2.38450;,
 0.00000;36.88788;2.38450;,
 -3.80130;37.48997;2.38450;,
 -7.23040;39.23717;2.38450;,
 -9.95190;41.95868;2.38450;,
 -11.69910;45.38778;2.38450;,
 -12.30120;49.18908;2.38450;,
 -11.69910;52.99038;-75.89050;,
 -9.95190;56.41957;-75.89050;,
 -7.88150;54.78398;-75.89050;,
 -9.09380;52.00898;-75.89050;,
 -7.23050;59.14098;-75.89050;,
 -5.69200;56.89778;-75.89050;,
 -3.80130;60.88818;-75.89050;,
 -3.12060;58.68258;-75.89050;,
 0.00000;61.49028;-75.89050;,
 0.00870;59.32338;-75.89050;,
 3.80130;60.88818;-75.89050;,
 3.23520;58.98148;-75.89050;,
 7.23040;59.14098;-75.89050;,
 6.13520;57.48388;-75.89050;,
 9.95190;56.41957;-75.89050;,
 8.29210;55.06968;-75.89050;,
 11.69910;52.99038;-75.89050;,
 9.36630;52.09198;-75.89050;,
 12.30120;49.18908;-75.89050;,
 9.53600;49.05148;-75.89050;,
 11.69910;45.38778;-75.89050;,
 8.93970;46.14958;-75.89050;,
 9.95190;41.95868;-75.89050;,
 7.78450;43.40198;-75.89050;,
 7.23040;39.23717;-75.89050;,
 5.55250;41.42097;-75.89050;,
 3.80130;37.48997;-75.89050;,
 2.89980;40.15368;-75.89050;,
 0.00000;36.88788;-75.89050;,
 0.00870;39.71408;-75.89050;,
 -3.80130;37.48997;-75.89050;,
 -2.88810;40.13618;-75.89050;,
 -7.23040;39.23717;-75.89050;,
 -5.83420;41.00948;-75.89050;,
 -9.95190;41.95868;-75.89050;,
 -8.46550;42.89458;-75.89050;,
 -11.69910;45.38778;-75.89050;,
 -9.50390;45.96067;-75.89050;,
 -12.30120;49.18908;-75.89050;,
 -9.57760;49.05148;-75.89050;,
 0.00870;59.32338;-68.62190;,
 3.23520;58.98148;-68.62190;,
 0.00000;49.18908;-68.62190;,
 -3.12070;58.68258;-68.62190;,
 -5.69200;56.89778;-68.62190;,
 -7.88150;54.78398;-68.62190;,
 -9.09380;52.00898;-68.62190;,
 -9.57760;49.05148;-68.62190;,
 -9.50390;45.96067;-68.62190;,
 -8.46550;42.89458;-68.62190;,
 -5.83420;41.00948;-68.62190;,
 -2.88810;40.13618;-68.62190;,
 0.00870;39.71408;-68.62190;,
 2.89980;40.15368;-68.62190;,
 5.55250;41.42097;-68.62190;,
 7.78450;43.40198;-68.62190;,
 8.93970;46.14958;-68.62190;,
 9.53600;49.05148;-68.62190;,
 9.36630;52.09198;-68.62190;,
 8.29210;55.06968;-68.62190;,
 6.13520;57.48388;-68.62190;;
 
 210;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;2,5,6,7;,
 4;3,2,7,8;,
 4;9,10,11,12;,
 4;10,0,3,11;,
 4;11,3,8,13;,
 4;12,11,13,14;,
 4;14,13,15,16;,
 4;13,8,17,15;,
 4;15,17,18,19;,
 4;16,15,19,20;,
 4;8,7,21,17;,
 4;7,6,22,21;,
 4;21,22,23,24;,
 4;17,21,24,18;,
 4;18,24,25,26;,
 4;24,23,27,25;,
 4;25,27,28,29;,
 4;26,25,29,30;,
 4;20,19,31,32;,
 4;19,18,26,31;,
 4;31,26,30,33;,
 4;32,31,33,34;,
 4;34,33,35,36;,
 4;33,30,37,35;,
 4;35,37,38,39;,
 4;36,35,39,40;,
 4;30,29,41,37;,
 4;29,28,42,41;,
 4;41,42,43,44;,
 4;37,41,44,38;,
 4;45,46,47,48;,
 4;46,49,50,47;,
 4;47,50,51,52;,
 4;48,47,52,53;,
 4;4,54,55,5;,
 4;54,45,48,55;,
 4;55,48,53,56;,
 4;5,55,56,6;,
 4;57,58,59,60;,
 4;58,9,12,59;,
 4;59,12,14,61;,
 4;60,59,61,62;,
 4;63,64,65,66;,
 4;64,57,60,65;,
 4;65,60,62,67;,
 4;66,65,67,68;,
 3;69,70,71;,
 3;70,72,71;,
 3;71,72,73;,
 3;72,74,73;,
 3;73,74,75;,
 3;74,76,75;,
 3;75,76,77;,
 3;76,78,77;,
 3;77,78,79;,
 3;78,80,79;,
 3;79,80,81;,
 3;80,82,81;,
 3;81,82,83;,
 3;82,84,83;,
 4;85,86,87,88;,
 4;86,89,90,87;,
 4;89,91,92,90;,
 4;91,93,94,92;,
 4;93,95,96,94;,
 4;95,97,98,96;,
 4;97,99,100,98;,
 4;99,101,102,100;,
 4;103,104,105,106;,
 4;107,103,106,108;,
 4;109,107,108,110;,
 4;111,109,110,112;,
 4;113,114,115,116;,
 4;114,117,118,115;,
 4;117,119,120,118;,
 4;119,85,88,120;,
 4;73,75,86,85;,
 4;10,9,88,87;,
 4;75,77,89,86;,
 4;0,10,87,90;,
 4;77,79,91,89;,
 4;1,0,90,92;,
 4;79,81,93,91;,
 4;4,1,92,94;,
 4;81,83,95,93;,
 4;54,4,94,96;,
 4;83,84,97,95;,
 4;45,54,96,98;,
 4;84,82,99,97;,
 4;46,45,98,100;,
 4;82,80,101,99;,
 4;49,46,100,102;,
 4;44,43,104,103;,
 4;121,78,106,105;,
 4;38,44,103,107;,
 4;78,76,108,106;,
 4;39,38,107,109;,
 4;76,74,110,108;,
 4;40,39,109,111;,
 4;74,72,112,110;,
 4;122,70,114,113;,
 4;64,63,116,115;,
 4;70,69,117,114;,
 4;57,64,115,118;,
 4;69,71,119,117;,
 4;58,57,118,120;,
 4;71,73,85,119;,
 4;9,58,120,88;,
 4;123,124,125,126;,
 4;124,127,128,125;,
 4;127,129,130,128;,
 4;129,131,132,130;,
 4;131,133,134,132;,
 4;133,135,136,134;,
 4;135,137,138,136;,
 4;137,139,140,138;,
 4;139,141,142,140;,
 4;141,143,144,142;,
 4;143,145,146,144;,
 4;145,147,148,146;,
 4;147,149,150,148;,
 4;149,151,152,150;,
 4;151,153,154,152;,
 4;153,155,156,154;,
 4;155,157,158,156;,
 4;157,159,160,158;,
 4;159,161,162,160;,
 4;161,163,164,162;,
 3;165,166,167;,
 3;168,165,167;,
 3;169,168,167;,
 3;170,169,167;,
 3;171,170,167;,
 3;172,171,167;,
 3;173,172,167;,
 3;174,173,167;,
 3;175,174,167;,
 3;176,175,167;,
 3;177,176,167;,
 3;178,177,167;,
 3;179,178,167;,
 3;180,179,167;,
 3;181,180,167;,
 3;182,181,167;,
 3;183,182,167;,
 3;184,183,167;,
 3;185,184,167;,
 3;166,185,167;,
 4;186,187,188,189;,
 4;187,190,191,188;,
 4;190,192,193,191;,
 4;192,194,195,193;,
 4;194,196,197,195;,
 4;196,198,199,197;,
 4;198,200,201,199;,
 4;200,202,203,201;,
 4;202,204,205,203;,
 4;204,206,207,205;,
 4;206,208,209,207;,
 4;208,210,211,209;,
 4;210,212,213,211;,
 4;212,214,215,213;,
 4;214,216,217,215;,
 4;216,218,219,217;,
 4;218,220,221,219;,
 4;220,222,223,221;,
 4;222,224,225,223;,
 4;224,186,189,225;,
 3;226,227,228;,
 3;229,226,228;,
 3;230,229,228;,
 3;231,230,228;,
 3;232,231,228;,
 3;233,232,228;,
 3;234,233,228;,
 3;235,234,228;,
 3;236,235,228;,
 3;237,236,228;,
 3;238,237,228;,
 3;239,238,228;,
 3;240,239,228;,
 3;241,240,228;,
 3;242,241,228;,
 3;243,242,228;,
 3;244,243,228;,
 3;245,244,228;,
 3;246,245,228;,
 3;227,246,228;,
 4;195,197,227,226;,
 4;193,195,226,229;,
 4;191,193,229,230;,
 4;188,191,230,231;,
 4;189,188,231,232;,
 4;225,189,232,233;,
 4;223,225,233,234;,
 4;221,223,234,235;,
 4;219,221,235,236;,
 4;217,219,236,237;,
 4;215,217,237,238;,
 4;213,215,238,239;,
 4;211,213,239,240;,
 4;209,211,240,241;,
 4;207,209,241,242;,
 4;205,207,242,243;,
 4;203,205,243,244;,
 4;201,203,244,245;,
 4;199,201,245,246;,
 4;197,199,246,227;;
 
 MeshMaterialList {
  3;
  210;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  1,
  0,
  1,
  0,
  1,
  0,
  1,
  0,
  1,
  0,
  1,
  0,
  1,
  0,
  1,
  1,
  0,
  1,
  0,
  1,
  0,
  1,
  0,
  0,
  1,
  0,
  1,
  0,
  1,
  0,
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
  1;;
  Material {
   0.160000;0.160000;0.160000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.796800;0.122400;1.000000;;
   23.000000;
   0.280000;0.280000;0.280000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.248000;0.248000;0.248000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  163;
  0.657108;0.369352;-0.657106;,
  -0.657108;0.369352;-0.657106;,
  0.657109;0.369351;0.657106;,
  -0.657109;0.369351;0.657106;,
  0.861643;0.507514;-0.000001;,
  0.032046;-0.998973;-0.032046;,
  -0.032046;-0.998973;-0.032046;,
  0.032045;-0.998973;0.032045;,
  0.000000;0.507515;-0.861643;,
  -0.032045;-0.998973;0.032045;,
  -0.861643;0.507514;-0.000001;,
  0.000000;0.507515;0.861643;,
  0.000000;-0.998899;0.046917;,
  0.000000;1.000000;-0.000001;,
  0.000000;-0.998899;-0.046917;,
  0.078191;-0.996938;0.000000;,
  -0.078191;-0.996938;0.000000;,
  -0.371054;0.210816;-0.904365;,
  0.371054;0.210817;-0.904365;,
  0.497014;0.711305;-0.497014;,
  -0.497014;0.711305;-0.497014;,
  -0.497015;0.711304;0.497014;,
  0.497015;0.711304;0.497014;,
  0.371055;0.210815;0.904364;,
  -0.371055;0.210815;0.904364;,
  -0.904364;0.210817;0.371056;,
  -0.904364;0.210817;-0.371056;,
  0.904364;0.210816;-0.371056;,
  0.904364;0.210817;0.371056;,
  0.015995;-0.998927;0.043457;,
  -0.000000;0.211777;-0.977318;,
  -0.015995;-0.998927;0.043457;,
  0.000000;0.841689;-0.539962;,
  -0.539962;0.841689;-0.000001;,
  0.000000;0.841690;0.539961;,
  0.539962;0.841690;-0.000001;,
  -0.000000;0.211776;0.977318;,
  0.015995;-0.998927;-0.043457;,
  -0.015995;-0.998927;-0.043457;,
  0.043457;-0.998927;-0.015995;,
  -0.977318;0.211777;-0.000001;,
  0.054326;-0.998323;0.019996;,
  -0.043457;-0.998927;0.015995;,
  0.977318;0.211777;-0.000001;,
  -0.054327;-0.998323;-0.019996;,
  -0.691223;0.210769;-0.691221;,
  -0.422251;0.453187;-0.785064;,
  0.422251;0.453187;-0.785064;,
  0.691223;0.210769;-0.691221;,
  -0.785065;0.453186;-0.422251;,
  -0.785065;0.453186;0.422251;,
  -0.422252;0.453187;0.785064;,
  0.422252;0.453187;0.785064;,
  0.785065;0.453186;0.422251;,
  0.785065;0.453186;-0.422251;,
  -0.691223;0.210768;0.691222;,
  0.691223;0.210768;0.691222;,
  0.702831;0.109804;-0.702831;,
  0.354490;0.103304;-0.929336;,
  -0.000000;0.097599;-0.995226;,
  -0.354490;0.103304;-0.929336;,
  -0.702831;0.109804;-0.702831;,
  -0.929335;0.103304;-0.354493;,
  -0.995226;0.097599;-0.000000;,
  -0.929335;0.103304;0.354493;,
  -0.702831;0.109802;0.702832;,
  -0.354491;0.103301;0.929336;,
  0.000000;0.097596;0.995226;,
  0.354491;0.103301;0.929336;,
  0.702831;0.109802;0.702832;,
  0.929335;0.103304;0.354493;,
  0.995226;0.097599;-0.000001;,
  0.929335;0.103304;-0.354493;,
  0.347331;0.103308;-0.932035;,
  0.702814;0.110026;-0.702814;,
  0.000000;0.097500;-0.995236;,
  -0.347331;0.103308;-0.932035;,
  -0.702814;0.110026;-0.702814;,
  -0.932034;0.103306;-0.347332;,
  -0.995236;0.097496;0.000000;,
  -0.932035;0.103305;0.347332;,
  -0.702814;0.110023;0.702814;,
  -0.347332;0.103306;0.932034;,
  0.000000;0.097501;0.995236;,
  0.347332;0.103306;0.932034;,
  0.702814;0.110023;0.702814;,
  0.932035;0.103305;0.347332;,
  0.995236;0.097496;0.000000;,
  0.932034;0.103306;-0.347332;,
  -0.951057;0.309014;0.000000;,
  -0.809021;0.587779;0.000000;,
  -0.587779;0.809021;0.000000;,
  -0.309014;0.951057;0.000000;,
  0.000000;1.000000;0.000000;,
  0.309020;0.951056;0.000000;,
  0.587777;0.809023;0.000000;,
  0.809016;0.587787;0.000000;,
  0.951057;0.309014;0.000000;,
  1.000000;0.000000;0.000000;,
  0.951056;-0.309020;-0.000000;,
  0.809018;-0.587784;-0.000000;,
  0.587784;-0.809018;-0.000000;,
  0.309019;-0.951056;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.309019;-0.951056;0.000000;,
  -0.587784;-0.809018;0.000000;,
  -0.809018;-0.587784;0.000000;,
  -0.951056;-0.309020;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  -0.039929;-0.993296;0.108483;,
  -0.080009;-0.993578;0.080009;,
  0.003980;0.993657;0.112379;,
  -0.059343;0.996472;0.059344;,
  0.000000;-0.993119;0.117113;,
  -0.000000;0.992806;0.119733;,
  0.039929;-0.993296;0.108483;,
  -0.003976;0.993656;0.112389;,
  0.080009;-0.993578;0.080009;,
  0.059343;0.996472;0.059344;,
  0.108484;-0.993296;0.039930;,
  0.112378;0.993658;-0.003980;,
  0.117114;-0.993118;0.000000;,
  0.119726;0.992807;-0.000000;,
  0.108484;-0.993296;-0.039930;,
  0.112378;0.993658;0.003980;,
  0.080010;-0.993578;-0.080010;,
  0.059343;0.996472;-0.059343;,
  -0.003976;0.993656;-0.112388;,
  0.039930;-0.993296;-0.108483;,
  0.000000;0.992805;-0.119739;,
  0.000000;-0.993119;-0.117112;,
  0.003976;0.993656;-0.112388;,
  -0.039930;-0.993296;-0.108483;,
  -0.059343;0.996472;-0.059343;,
  -0.080010;-0.993578;-0.080010;,
  -0.108484;-0.993296;-0.039930;,
  -0.112378;0.993658;0.003980;,
  -0.117114;-0.993118;0.000000;,
  -0.119733;0.992806;-0.000000;,
  -0.108484;-0.993296;0.039930;,
  -0.112388;0.993656;-0.003976;,
  0.048185;-0.998838;0.000001;,
  -0.287037;-0.957919;0.000000;,
  0.393440;-0.919350;0.000003;,
  0.634441;-0.772971;0.000002;,
  0.821116;-0.570761;-0.000000;,
  0.959094;-0.283088;-0.000000;,
  0.997610;-0.069099;-0.000000;,
  0.984693;0.174299;0.000000;,
  0.803382;0.595464;0.000000;,
  0.439387;0.898298;0.000000;,
  0.214748;0.976669;0.000000;,
  -0.003101;0.999995;0.000000;,
  -0.293885;0.955841;0.000000;,
  -0.552855;0.833278;0.000000;,
  -0.813033;0.582217;0.000000;,
  -0.955238;0.295839;0.000000;,
  -0.997303;0.073389;0.000000;,
  -0.979870;-0.199636;0.000000;,
  -0.858892;-0.512156;0.000000;,
  -0.612453;-0.790507;0.000000;;
  210;
  4;59,60,17,30;,
  4;60,61,45,17;,
  4;17,45,1,46;,
  4;30,17,46,8;,
  4;57,58,18,48;,
  4;58,59,30,18;,
  4;18,30,8,47;,
  4;48,18,47,0;,
  4;0,47,19,54;,
  4;47,8,32,19;,
  4;19,32,13,35;,
  4;54,19,35,4;,
  4;8,46,20,32;,
  4;46,1,49,20;,
  4;20,49,10,33;,
  4;32,20,33,13;,
  4;13,33,21,34;,
  4;33,10,50,21;,
  4;21,50,3,51;,
  4;34,21,51,11;,
  4;4,35,22,53;,
  4;35,13,34,22;,
  4;22,34,11,52;,
  4;53,22,52,2;,
  4;2,52,23,56;,
  4;52,11,36,23;,
  4;23,36,67,68;,
  4;56,23,68,69;,
  4;11,51,24,36;,
  4;51,3,55,24;,
  4;24,55,65,66;,
  4;36,24,66,67;,
  4;63,64,25,40;,
  4;64,65,55,25;,
  4;25,55,3,50;,
  4;40,25,50,10;,
  4;61,62,26,45;,
  4;62,63,40,26;,
  4;26,40,10,49;,
  4;45,26,49,1;,
  4;71,72,27,43;,
  4;72,57,48,27;,
  4;27,48,0,54;,
  4;43,27,54,4;,
  4;69,70,28,56;,
  4;70,71,43,28;,
  4;28,43,4,53;,
  4;56,28,53,2;,
  3;16,44,42;,
  3;44,6,42;,
  3;42,6,9;,
  3;6,38,9;,
  3;9,38,31;,
  3;38,14,31;,
  3;31,14,12;,
  3;14,37,12;,
  3;12,37,29;,
  3;37,5,29;,
  3;29,5,7;,
  3;5,39,7;,
  3;7,39,41;,
  3;39,15,41;,
  4;74,73,73,74;,
  4;73,75,75,73;,
  4;75,76,76,75;,
  4;76,77,77,76;,
  4;77,78,78,77;,
  4;78,79,79,78;,
  4;79,80,80,79;,
  4;80,81,81,80;,
  4;82,81,81,82;,
  4;83,82,82,83;,
  4;84,83,83,84;,
  4;85,84,84,85;,
  4;85,86,86,85;,
  4;86,87,87,86;,
  4;87,88,88,87;,
  4;88,74,74,88;,
  4;9,31,111,112;,
  4;113,114,114,113;,
  4;31,12,115,111;,
  4;116,113,113,116;,
  4;12,29,117,115;,
  4;118,116,116,118;,
  4;29,7,119,117;,
  4;120,118,118,120;,
  4;7,41,121,119;,
  4;122,120,120,122;,
  4;41,15,123,121;,
  4;124,122,122,124;,
  4;15,39,125,123;,
  4;126,124,124,126;,
  4;39,5,127,125;,
  4;128,126,126,128;,
  4;129,128,128,129;,
  4;5,37,130,127;,
  4;131,129,129,131;,
  4;37,14,132,130;,
  4;133,131,131,133;,
  4;14,38,134,132;,
  4;135,133,133,135;,
  4;38,6,136,134;,
  4;6,44,137,136;,
  4;138,135,135,138;,
  4;44,16,139,137;,
  4;140,138,138,140;,
  4;16,42,141,139;,
  4;142,140,140,142;,
  4;42,9,112,141;,
  4;114,142,142,114;,
  4;89,90,90,89;,
  4;90,91,91,90;,
  4;91,92,92,91;,
  4;92,93,93,92;,
  4;93,94,94,93;,
  4;94,95,95,94;,
  4;95,96,96,95;,
  4;96,97,97,96;,
  4;97,98,98,97;,
  4;98,99,99,98;,
  4;99,100,100,99;,
  4;100,101,101,100;,
  4;101,102,102,101;,
  4;102,103,103,102;,
  4;103,104,104,103;,
  4;104,105,105,104;,
  4;105,106,106,105;,
  4;106,107,107,106;,
  4;107,108,108,107;,
  4;108,89,89,108;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  3;109,109,109;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  4;110,110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  3;110,110,110;,
  4;143,144,144,143;,
  4;145,143,143,145;,
  4;146,145,145,146;,
  4;147,146,146,147;,
  4;148,147,147,148;,
  4;149,148,148,149;,
  4;150,149,149,150;,
  4;151,150,150,151;,
  4;152,151,151,152;,
  4;153,152,152,153;,
  4;154,153,153,154;,
  4;155,154,154,155;,
  4;156,155,155,156;,
  4;157,156,156,157;,
  4;158,157,157,158;,
  4;159,158,158,159;,
  4;160,159,159,160;,
  4;161,160,160,161;,
  4;162,161,161,162;,
  4;144,162,162,144;;
 }
 MeshTextureCoords {
  247;
  0.500000;0.145610;,
  0.562500;0.145610;,
  0.562500;0.187500;,
  0.500000;0.187500;,
  0.625000;0.145610;,
  0.625000;0.187500;,
  0.625000;0.250000;,
  0.562500;0.250000;,
  0.500000;0.250000;,
  0.375000;0.145610;,
  0.437500;0.145610;,
  0.437500;0.187500;,
  0.375000;0.187500;,
  0.437500;0.250000;,
  0.375000;0.250000;,
  0.437500;0.312500;,
  0.375000;0.312500;,
  0.500000;0.312500;,
  0.500000;0.375000;,
  0.437500;0.375000;,
  0.375000;0.375000;,
  0.562500;0.312500;,
  0.625000;0.312500;,
  0.625000;0.375000;,
  0.562500;0.375000;,
  0.562500;0.437500;,
  0.500000;0.437500;,
  0.625000;0.437500;,
  0.625000;0.500000;,
  0.562500;0.500000;,
  0.500000;0.500000;,
  0.437500;0.437500;,
  0.375000;0.437500;,
  0.437500;0.500000;,
  0.375000;0.500000;,
  0.437500;0.562500;,
  0.375000;0.562500;,
  0.500000;0.562500;,
  0.500000;0.604390;,
  0.437500;0.604390;,
  0.375000;0.604390;,
  0.562500;0.562500;,
  0.625000;0.562500;,
  0.625000;0.604390;,
  0.562500;0.604390;,
  0.750000;0.145610;,
  0.812500;0.145610;,
  0.812500;0.187500;,
  0.750000;0.187500;,
  0.875000;0.145610;,
  0.875000;0.187500;,
  0.875000;0.250000;,
  0.812500;0.250000;,
  0.750000;0.250000;,
  0.687500;0.145610;,
  0.687500;0.187500;,
  0.687500;0.250000;,
  0.250000;0.145610;,
  0.312500;0.145610;,
  0.312500;0.187500;,
  0.250000;0.187500;,
  0.312500;0.250000;,
  0.250000;0.250000;,
  0.125000;0.145610;,
  0.187500;0.145610;,
  0.187500;0.187500;,
  0.125000;0.187500;,
  0.187500;0.250000;,
  0.125000;0.250000;,
  0.250000;0.125000;,
  0.187500;0.125000;,
  0.312500;0.125000;,
  0.375000;0.625000;,
  0.375000;0.125000;,
  0.437500;0.625000;,
  0.437500;0.125000;,
  0.500000;0.625000;,
  0.500000;0.125000;,
  0.562500;0.625000;,
  0.562500;0.125000;,
  0.875000;0.125000;,
  0.625000;0.125000;,
  0.812500;0.125000;,
  0.687500;0.125000;,
  0.750000;0.125000;,
  0.375000;0.125000;,
  0.437500;0.125000;,
  0.437500;0.145610;,
  0.375000;0.145610;,
  0.500000;0.125000;,
  0.500000;0.145610;,
  0.562500;0.125000;,
  0.562500;0.145610;,
  0.625000;0.125000;,
  0.625000;0.145610;,
  0.687500;0.125000;,
  0.687500;0.145610;,
  0.750000;0.125000;,
  0.750000;0.145610;,
  0.812500;0.125000;,
  0.812500;0.145610;,
  0.875000;0.125000;,
  0.875000;0.145610;,
  0.562500;0.604390;,
  0.625000;0.604390;,
  0.625000;0.625000;,
  0.562500;0.625000;,
  0.500000;0.604390;,
  0.500000;0.625000;,
  0.437500;0.604390;,
  0.437500;0.625000;,
  0.375000;0.604390;,
  0.375000;0.625000;,
  0.125000;0.125000;,
  0.187500;0.125000;,
  0.187500;0.145610;,
  0.125000;0.145610;,
  0.250000;0.125000;,
  0.250000;0.145610;,
  0.312500;0.125000;,
  0.312500;0.145610;,
  0.625000;0.625000;,
  0.125000;0.125000;,
  0.375000;0.312500;,
  0.387500;0.312500;,
  0.387500;0.688440;,
  0.375000;0.688440;,
  0.400000;0.312500;,
  0.400000;0.688440;,
  0.412500;0.312500;,
  0.412500;0.688440;,
  0.425000;0.312500;,
  0.425000;0.688440;,
  0.437500;0.312500;,
  0.437500;0.688440;,
  0.450000;0.312500;,
  0.450000;0.688440;,
  0.462500;0.312500;,
  0.462500;0.688440;,
  0.475000;0.312500;,
  0.475000;0.688440;,
  0.487500;0.312500;,
  0.487500;0.688440;,
  0.500000;0.312500;,
  0.500000;0.688440;,
  0.512500;0.312500;,
  0.512500;0.688440;,
  0.525000;0.312500;,
  0.525000;0.688440;,
  0.537500;0.312500;,
  0.537500;0.688440;,
  0.550000;0.312500;,
  0.550000;0.688440;,
  0.562500;0.312500;,
  0.562500;0.688440;,
  0.575000;0.312500;,
  0.575000;0.688440;,
  0.587500;0.312500;,
  0.587500;0.688440;,
  0.600000;0.312500;,
  0.600000;0.688440;,
  0.612500;0.312500;,
  0.612500;0.688440;,
  0.625000;0.312500;,
  0.625000;0.688440;,
  0.626410;0.064410;,
  0.648600;0.107970;,
  0.500000;0.150000;,
  0.591840;0.029840;,
  0.548280;0.007650;,
  0.500000;-0.000000;,
  0.451720;0.007650;,
  0.408160;0.029840;,
  0.373590;0.064410;,
  0.351400;0.107970;,
  0.343750;0.156250;,
  0.351400;0.204530;,
  0.373590;0.248090;,
  0.408160;0.282660;,
  0.451720;0.304850;,
  0.500000;0.312500;,
  0.548280;0.304850;,
  0.591840;0.282660;,
  0.626410;0.248090;,
  0.648600;0.204530;,
  0.656250;0.156250;,
  0.648600;0.892030;,
  0.626410;0.935590;,
  0.570710;0.892370;,
  0.581570;0.867430;,
  0.591840;0.970160;,
  0.551090;0.911290;,
  0.548280;0.992350;,
  0.528040;0.927440;,
  0.500000;1.000000;,
  0.500000;0.933230;,
  0.451720;0.992350;,
  0.471090;0.930230;,
  0.408160;0.970160;,
  0.445100;0.916800;,
  0.373590;0.935590;,
  0.425770;0.895100;,
  0.351400;0.892030;,
  0.416140;0.868270;,
  0.343750;0.843750;,
  0.414620;0.840920;,
  0.351400;0.795470;,
  0.419960;0.814860;,
  0.373590;0.751910;,
  0.430320;0.790320;,
  0.408160;0.717340;,
  0.450320;0.772500;,
  0.451720;0.695150;,
  0.474090;0.761120;,
  0.500000;0.687500;,
  0.500000;0.757170;,
  0.548280;0.695150;,
  0.525960;0.760970;,
  0.591840;0.717340;,
  0.552360;0.768990;,
  0.626410;0.751910;,
  0.575940;0.786080;,
  0.648600;0.795470;,
  0.585250;0.813390;,
  0.656250;0.843750;,
  0.585910;0.840940;,
  0.500000;0.933230;,
  0.471090;0.930230;,
  0.500000;0.837500;,
  0.528040;0.927440;,
  0.551090;0.911290;,
  0.570710;0.892370;,
  0.581570;0.867430;,
  0.585910;0.840940;,
  0.585250;0.813390;,
  0.575940;0.786080;,
  0.552360;0.768990;,
  0.525960;0.760970;,
  0.500000;0.757170;,
  0.474090;0.761120;,
  0.450320;0.772500;,
  0.430320;0.790320;,
  0.419960;0.814860;,
  0.414620;0.840920;,
  0.416140;0.868270;,
  0.425770;0.895100;,
  0.445100;0.916800;;
 }
}
