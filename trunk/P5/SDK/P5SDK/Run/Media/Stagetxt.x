xof 0303txt 0032
template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}

template VertexDuplicationIndices {
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}

template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}


Frame Scene_Root {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame Box01 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,4.412319,-1.800583,0.000000,1.000000;;
  }

  Frame {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }

   Mesh {
    24;
    -76.324677;-40.261703;0.000000;,
    76.324677;-40.261703;0.000000;,
    -76.324677;40.261703;0.000000;,
    76.324677;40.261703;0.000000;,
    -76.324677;-40.261703;-5.474305;,
    76.324677;-40.261703;-5.474305;,
    -76.324677;40.261703;-5.474305;,
    76.324677;40.261703;-5.474305;,
    76.324677;-40.261703;0.000000;,
    -76.324677;-40.261703;0.000000;,
    76.324677;-40.261703;-5.474305;,
    -76.324677;-40.261703;-5.474305;,
    76.324677;40.261703;0.000000;,
    76.324677;-40.261703;0.000000;,
    76.324677;40.261703;-5.474305;,
    76.324677;-40.261703;-5.474305;,
    -76.324677;40.261703;0.000000;,
    76.324677;40.261703;0.000000;,
    -76.324677;40.261703;-5.474305;,
    76.324677;40.261703;-5.474305;,
    -76.324677;-40.261703;0.000000;,
    -76.324677;40.261703;0.000000;,
    -76.324677;-40.261703;-5.474305;,
    -76.324677;40.261703;-5.474305;;
    12;
    3;2,0,3;,
    3;1,3,0;,
    3;5,4,7;,
    3;6,7,4;,
    3;8,9,10;,
    3;11,10,9;,
    3;12,13,14;,
    3;15,14,13;,
    3;16,17,18;,
    3;19,18,17;,
    3;20,21,22;,
    3;23,22,21;;

    MeshNormals {
     24;
     0.000000;0.000000;2.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-2.000000;,
     0.000000;-1.000000;0.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     -1.000000;0.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;;
     12;
     3;2,0,3;,
     3;1,3,0;,
     3;5,4,7;,
     3;6,7,4;,
     3;8,9,10;,
     3;11,10,9;,
     3;12,13,14;,
     3;15,14,13;,
     3;16,17,18;,
     3;19,18,17;,
     3;20,21,22;,
     3;23,22,21;;
    }

    MeshTextureCoords {
     24;
     0.000000;0.000000;,
     1.000000;0.000000;,
     0.000000;-1.000000;,
     1.000000;-1.000000;,
     1.000000;0.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     1.000000;0.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     1.000000;0.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     1.000000;0.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     1.000000;0.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;;
    }

    VertexDuplicationIndices {
     24;
     8;
     0,
     1,
     2,
     3,
     4,
     5,
     6,
     7,
     1,
     0,
     5,
     4,
     3,
     1,
     7,
     5,
     2,
     3,
     6,
     7,
     0,
     2,
     4,
     6;
    }

    MeshMaterialList {
     1;
     12;
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
     0;

     Material {
      1.000000;1.000000;1.000000;1.000000;;
      0.000000;
      1.000000;1.000000;1.000000;;
      0.000000;0.000000;0.000000;;

      TextureFilename {
       "media\\CEDFENCE.JPG";
      }
     }
    }
   }
  }
 }

 Frame Box02 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,4.339180,36.106716,30.114172,1.000000;;
  }

  Frame {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,-0.000000,1.000000,0.000000,0.000000,0.000000,0.000002,1.000000;;
   }

   Mesh {
    24;
    -73.640167;-30.125523;0.000000;,
    73.640167;-30.125523;0.000000;,
    -73.640167;30.125523;0.000000;,
    73.640167;30.125523;0.000000;,
    -73.640167;-30.125523;3.347281;,
    73.640167;-30.125523;3.347281;,
    -73.640167;30.125523;3.347281;,
    73.640167;30.125523;3.347281;,
    -73.640167;-30.125523;0.000000;,
    73.640167;-30.125523;0.000000;,
    73.640167;-30.125523;3.347281;,
    -73.640167;-30.125523;3.347281;,
    73.640167;-30.125523;0.000000;,
    73.640167;30.125523;0.000000;,
    73.640167;30.125523;3.347281;,
    73.640167;-30.125523;3.347281;,
    73.640167;30.125523;0.000000;,
    -73.640167;30.125523;0.000000;,
    -73.640167;30.125523;3.347281;,
    73.640167;30.125523;3.347281;,
    -73.640167;30.125523;0.000000;,
    -73.640167;-30.125523;0.000000;,
    -73.640167;-30.125523;3.347281;,
    -73.640167;30.125523;3.347281;;
    12;
    3;0,2,3;,
    3;3,1,0;,
    3;4,5,7;,
    3;7,6,4;,
    3;8,9,10;,
    3;10,11,8;,
    3;12,13,14;,
    3;14,15,12;,
    3;16,17,18;,
    3;18,19,16;,
    3;20,21,22;,
    3;22,23,20;;

    MeshNormals {
     24;
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;;
     12;
     3;0,2,3;,
     3;3,1,0;,
     3;4,5,7;,
     3;7,6,4;,
     3;8,9,10;,
     3;10,11,8;,
     3;12,13,14;,
     3;14,15,12;,
     3;16,17,18;,
     3;18,19,16;,
     3;20,21,22;,
     3;22,23,20;;
    }

    MeshTextureCoords {
     24;
     1.000000;0.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     0.000000;-1.000000;,
     1.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;;
    }

    VertexDuplicationIndices {
     24;
     8;
     0,
     1,
     2,
     3,
     4,
     5,
     6,
     7,
     0,
     1,
     5,
     4,
     1,
     3,
     7,
     5,
     3,
     2,
     6,
     7,
     2,
     0,
     4,
     6;
    }

    MeshMaterialList {
     1;
     12;
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
     0;

     Material {
      1.000000;1.000000;1.000000;1.000000;;
      0.000000;
      1.000000;1.000000;1.000000;;
      0.000000;0.000000;0.000000;;

      TextureFilename {
       "media\\backpainting.jpg";
      }
     }
    }
   }
  }
 }

 Frame Box03 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-51.356995,-40.750980,0.000000,1.000000;;
  }

  Frame {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }

   Mesh {
    24;
    -17.118998;-0.835072;0.000000;,
    17.118998;-0.835072;0.000000;,
    -17.118998;0.835072;0.000000;,
    17.118998;0.835072;0.000000;,
    -17.118998;-0.835072;60.125263;,
    17.118998;-0.835072;60.125263;,
    -17.118998;0.835072;60.125263;,
    17.118998;0.835072;60.125263;,
    -17.118998;-0.835072;0.000000;,
    17.118998;-0.835072;0.000000;,
    17.118998;-0.835072;60.125263;,
    -17.118998;-0.835072;60.125263;,
    17.118998;-0.835072;0.000000;,
    17.118998;0.835072;0.000000;,
    17.118998;0.835072;60.125263;,
    17.118998;-0.835072;60.125263;,
    17.118998;0.835072;0.000000;,
    -17.118998;0.835072;0.000000;,
    -17.118998;0.835072;60.125263;,
    17.118998;0.835072;60.125263;,
    -17.118998;0.835072;0.000000;,
    -17.118998;-0.835072;0.000000;,
    -17.118998;-0.835072;60.125263;,
    -17.118998;0.835072;60.125263;;
    12;
    3;0,2,3;,
    3;3,1,0;,
    3;4,5,7;,
    3;7,6,4;,
    3;8,9,10;,
    3;10,11,8;,
    3;12,13,14;,
    3;14,15,12;,
    3;16,17,18;,
    3;18,19,16;,
    3;20,21,22;,
    3;22,23,20;;

    MeshNormals {
     24;
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;;
     12;
     3;0,2,3;,
     3;3,1,0;,
     3;4,5,7;,
     3;7,6,4;,
     3;8,9,10;,
     3;10,11,8;,
     3;12,13,14;,
     3;14,15,12;,
     3;16,17,18;,
     3;18,19,16;,
     3;20,21,22;,
     3;22,23,20;;
    }

    MeshTextureCoords {
     24;
     1.000000;0.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     0.000000;-1.000000;,
     1.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;;
    }

    VertexDuplicationIndices {
     24;
     8;
     0,
     1,
     2,
     3,
     4,
     5,
     6,
     7,
     0,
     1,
     5,
     4,
     1,
     3,
     7,
     5,
     3,
     2,
     6,
     7,
     2,
     0,
     4,
     6;
    }

    MeshMaterialList {
     1;
     12;
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
     0;

     Material {
      1.000000;1.000000;1.000000;1.000000;;
      0.000000;
      1.000000;1.000000;1.000000;;
      0.000000;0.000000;0.000000;;

      TextureFilename {
       "media\\curtainleftc.jpg";
      }
     }
    }
   }
  }
 }

 Frame Box04 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-67.774796,-3.303154,0.000000,1.000000;;
  }

  Frame {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }

   Mesh {
    24;
    -1.670147;-36.743217;0.000000;,
    1.670147;-36.743217;0.000000;,
    -1.670147;36.743217;0.000000;,
    1.670147;36.743217;0.000000;,
    -1.670147;-36.743217;60.125259;,
    1.670147;-36.743217;60.125259;,
    -1.670147;36.743217;60.125259;,
    1.670147;36.743217;60.125259;,
    -1.670147;-36.743217;0.000000;,
    1.670147;-36.743217;0.000000;,
    1.670147;-36.743217;60.125259;,
    -1.670147;-36.743217;60.125259;,
    1.670147;-36.743217;0.000000;,
    1.670147;36.743217;0.000000;,
    1.670147;36.743217;60.125259;,
    1.670147;-36.743217;60.125259;,
    1.670147;36.743217;0.000000;,
    -1.670147;36.743217;0.000000;,
    -1.670147;36.743217;60.125259;,
    1.670147;36.743217;60.125259;,
    -1.670147;36.743217;0.000000;,
    -1.670147;-36.743217;0.000000;,
    -1.670147;-36.743217;60.125259;,
    -1.670147;36.743217;60.125259;;
    12;
    3;0,2,3;,
    3;3,1,0;,
    3;4,5,7;,
    3;7,6,4;,
    3;8,9,10;,
    3;10,11,8;,
    3;12,13,14;,
    3;14,15,12;,
    3;16,17,18;,
    3;18,19,16;,
    3;20,21,22;,
    3;22,23,20;;

    MeshNormals {
     24;
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;;
     12;
     3;0,2,3;,
     3;3,1,0;,
     3;4,5,7;,
     3;7,6,4;,
     3;8,9,10;,
     3;10,11,8;,
     3;12,13,14;,
     3;14,15,12;,
     3;16,17,18;,
     3;18,19,16;,
     3;20,21,22;,
     3;22,23,20;;
    }

    MeshTextureCoords {
     24;
     1.000000;0.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     0.000000;-1.000000;,
     1.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;;
    }

    VertexDuplicationIndices {
     24;
     8;
     0,
     1,
     2,
     3,
     4,
     5,
     6,
     7,
     0,
     1,
     5,
     4,
     1,
     3,
     7,
     5,
     3,
     2,
     6,
     7,
     2,
     0,
     4,
     6;
    }

    MeshMaterialList {
     1;
     12;
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
     0;

     Material {
      1.000000;1.000000;1.000000;1.000000;;
      0.000000;
      1.000000;1.000000;1.000000;;
      0.000000;0.000000;0.000000;;

      TextureFilename {
       "media\\curtainrightc.jpg";
      }
     }
    }
   }
  }
 }

 Frame Box05 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,77.762573,-3.046958,0.000000,1.000000;;
  }

  Frame {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }

   Mesh {
    24;
    -1.670147;-36.743217;0.000000;,
    1.670147;-36.743217;0.000000;,
    -1.670147;36.743217;0.000000;,
    1.670147;36.743217;0.000000;,
    -1.670147;-36.743217;60.125259;,
    1.670147;-36.743217;60.125259;,
    -1.670147;36.743217;60.125259;,
    1.670147;36.743217;60.125259;,
    -1.670147;-36.743217;0.000000;,
    1.670147;-36.743217;0.000000;,
    1.670147;-36.743217;60.125259;,
    -1.670147;-36.743217;60.125259;,
    1.670147;-36.743217;0.000000;,
    1.670147;36.743217;0.000000;,
    1.670147;36.743217;60.125259;,
    1.670147;-36.743217;60.125259;,
    1.670147;36.743217;0.000000;,
    -1.670147;36.743217;0.000000;,
    -1.670147;36.743217;60.125259;,
    1.670147;36.743217;60.125259;,
    -1.670147;36.743217;0.000000;,
    -1.670147;-36.743217;0.000000;,
    -1.670147;-36.743217;60.125259;,
    -1.670147;36.743217;60.125259;;
    12;
    3;0,2,3;,
    3;3,1,0;,
    3;4,5,7;,
    3;7,6,4;,
    3;8,9,10;,
    3;10,11,8;,
    3;12,13,14;,
    3;14,15,12;,
    3;16,17,18;,
    3;18,19,16;,
    3;20,21,22;,
    3;22,23,20;;

    MeshNormals {
     24;
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;;
     12;
     3;0,2,3;,
     3;3,1,0;,
     3;4,5,7;,
     3;7,6,4;,
     3;8,9,10;,
     3;10,11,8;,
     3;12,13,14;,
     3;14,15,12;,
     3;16,17,18;,
     3;18,19,16;,
     3;20,21,22;,
     3;22,23,20;;
    }

    MeshTextureCoords {
     24;
     1.000000;0.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     0.000000;-1.000000;,
     1.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;;
    }

    VertexDuplicationIndices {
     24;
     8;
     0,
     1,
     2,
     3,
     4,
     5,
     6,
     7,
     0,
     1,
     5,
     4,
     1,
     3,
     7,
     5,
     3,
     2,
     6,
     7,
     2,
     0,
     4,
     6;
    }

    MeshMaterialList {
     1;
     12;
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
     0;

     Material {
      1.000000;1.000000;1.000000;1.000000;;
      0.000000;
      1.000000;1.000000;1.000000;;
      0.000000;0.000000;0.000000;;

      TextureFilename {
       "media\\curtainrightc.jpg";
      }
     }
    }
   }
  }
 }

 Frame Box06 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,60.985413,-40.750980,0.000000,1.000000;;
  }

  Frame {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }

   Mesh {
    24;
    -17.118998;-0.835072;0.000000;,
    17.118998;-0.835072;0.000000;,
    -17.118998;0.835072;0.000000;,
    17.118998;0.835072;0.000000;,
    -17.118998;-0.835072;60.125263;,
    17.118998;-0.835072;60.125263;,
    -17.118998;0.835072;60.125263;,
    17.118998;0.835072;60.125263;,
    -17.118998;-0.835072;0.000000;,
    17.118998;-0.835072;0.000000;,
    17.118998;-0.835072;60.125263;,
    -17.118998;-0.835072;60.125263;,
    17.118998;-0.835072;0.000000;,
    17.118998;0.835072;0.000000;,
    17.118998;0.835072;60.125263;,
    17.118998;-0.835072;60.125263;,
    17.118998;0.835072;0.000000;,
    -17.118998;0.835072;0.000000;,
    -17.118998;0.835072;60.125263;,
    17.118998;0.835072;60.125263;,
    -17.118998;0.835072;0.000000;,
    -17.118998;-0.835072;0.000000;,
    -17.118998;-0.835072;60.125263;,
    -17.118998;0.835072;60.125263;;
    12;
    3;0,2,3;,
    3;3,1,0;,
    3;4,5,7;,
    3;7,6,4;,
    3;8,9,10;,
    3;10,11,8;,
    3;12,13,14;,
    3;14,15,12;,
    3;16,17,18;,
    3;18,19,16;,
    3;20,21,22;,
    3;22,23,20;;

    MeshNormals {
     24;
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;;
     12;
     3;0,2,3;,
     3;3,1,0;,
     3;4,5,7;,
     3;7,6,4;,
     3;8,9,10;,
     3;10,11,8;,
     3;12,13,14;,
     3;14,15,12;,
     3;16,17,18;,
     3;18,19,16;,
     3;20,21,22;,
     3;22,23,20;;
    }

    MeshTextureCoords {
     24;
     1.000000;0.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     0.000000;-1.000000;,
     1.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;;
    }

    VertexDuplicationIndices {
     24;
     8;
     0,
     1,
     2,
     3,
     4,
     5,
     6,
     7,
     0,
     1,
     5,
     4,
     1,
     3,
     7,
     5,
     3,
     2,
     6,
     7,
     2,
     0,
     4,
     6;
    }

    MeshMaterialList {
     1;
     12;
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
     0;

     Material {
      1.000000;1.000000;1.000000;1.000000;;
      0.000000;
      1.000000;1.000000;1.000000;;
      0.000000;0.000000;0.000000;;

      TextureFilename {
       "media\\curtainrightc.jpg";
      }
     }
    }
   }
  }
 }

 Frame Box07 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,4.589211,-47.886486,70.472237,1.000000;;
  }

  Frame {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,-0.000000,1.000000,0.000000,0.000000,-0.000003,0.000000,1.000000;;
   }

   Mesh {
    48;
    -75.943535;-10.500832;0.000000;,
    75.943535;-10.500832;0.000000;,
    -75.943535;10.500832;0.000000;,
    75.943535;10.500832;0.000000;,
    -75.943535;-10.500832;-87.006912;,
    75.943535;-10.500832;-87.006912;,
    -75.943535;10.500832;-87.006912;,
    75.943535;10.500832;-87.006912;,
    -71.386925;-10.500832;-2.770973;,
    71.386925;-10.500832;-2.770973;,
    71.386925;-10.500832;-83.893074;,
    -71.386925;-10.500832;-83.893066;,
    -71.386925;8.322510;-2.770973;,
    71.386925;8.322510;-2.770973;,
    -71.386925;8.322510;-83.893074;,
    71.386925;8.322510;-83.893074;,
    75.943535;-10.500832;0.000000;,
    -75.943535;-10.500832;0.000000;,
    75.943535;10.500832;0.000000;,
    75.943535;-10.500832;0.000000;,
    75.943535;10.500832;-87.006912;,
    75.943535;-10.500832;-87.006912;,
    -75.943535;10.500832;0.000000;,
    75.943535;10.500832;0.000000;,
    -75.943535;10.500832;-87.006912;,
    75.943535;10.500832;-87.006912;,
    -75.943535;-10.500832;0.000000;,
    -75.943535;10.500832;0.000000;,
    -75.943535;-10.500832;-87.006912;,
    -75.943535;10.500832;-87.006912;,
    -75.943535;-10.500832;-87.006912;,
    75.943535;-10.500832;-87.006912;,
    -71.386925;-10.500832;-2.770973;,
    -71.386925;-10.500832;-83.893066;,
    -71.386925;8.322510;-2.770973;,
    -71.386925;8.322510;-83.893074;,
    71.386925;-10.500832;-2.770973;,
    -71.386925;-10.500832;-2.770973;,
    71.386925;8.322510;-2.770973;,
    -71.386925;8.322510;-2.770973;,
    71.386925;8.322510;-2.770973;,
    71.386925;8.322510;-83.893074;,
    71.386925;-10.500832;-2.770973;,
    71.386925;-10.500832;-83.893074;,
    -71.386925;-10.500832;-83.893066;,
    71.386925;-10.500832;-83.893074;,
    -71.386925;8.322510;-83.893074;,
    71.386925;8.322510;-83.893074;;
    28;
    3;2,0,3;,
    3;1,3,0;,
    3;5,4,7;,
    3;6,7,4;,
    3;9,16,8;,
    3;17,8,16;,
    3;18,19,20;,
    3;21,20,19;,
    3;22,23,24;,
    3;25,24,23;,
    3;26,27,28;,
    3;29,28,27;,
    3;17,30,8;,
    3;31,16,9;,
    3;11,30,10;,
    3;31,10,30;,
    3;31,9,10;,
    3;8,30,11;,
    3;12,14,13;,
    3;15,13,14;,
    3;32,33,34;,
    3;35,34,33;,
    3;36,37,38;,
    3;39,38,37;,
    3;40,41,42;,
    3;42,41,43;,
    3;44,45,46;,
    3;47,46,45;;

    MeshNormals {
     48;
     0.000000;0.000000;2.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-2.000000;,
     0.000000;-4.000000;0.000000;,
     0.000000;-3.000000;0.000000;,
     0.000000;-3.000000;0.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-2.000000;0.000000;,
     0.000000;-1.000000;0.000000;,
     0.000000;-3.000000;0.000000;,
     0.000000;-2.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;2.000000;0.000000;,
     0.000000;1.000000;0.000000;,
     -1.000000;0.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;,
     0.000000;-4.000000;0.000000;,
     0.000000;-3.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     2.000000;0.000000;0.000000;,
     1.000000;0.000000;0.000000;,
     0.000000;0.000000;-1.000000;,
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;-2.000000;,
     0.000000;0.000000;-1.000000;,
     -1.000000;0.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -2.000000;0.000000;0.000000;,
     -1.000000;0.000000;0.000000;,
     0.000000;0.000000;1.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;0.000000;2.000000;,
     0.000000;0.000000;1.000000;;
     28;
     3;2,0,3;,
     3;1,3,0;,
     3;5,4,7;,
     3;6,7,4;,
     3;9,16,8;,
     3;17,8,16;,
     3;18,19,20;,
     3;21,20,19;,
     3;22,23,24;,
     3;25,24,23;,
     3;26,27,28;,
     3;29,28,27;,
     3;17,30,8;,
     3;31,16,9;,
     3;11,30,10;,
     3;31,10,30;,
     3;31,9,10;,
     3;8,30,11;,
     3;12,14,13;,
     3;15,13,14;,
     3;32,33,34;,
     3;35,34,33;,
     3;36,37,38;,
     3;39,38,37;,
     3;40,41,42;,
     3;42,41,43;,
     3;44,45,46;,
     3;47,46,45;;
    }

    MeshTextureCoords {
     48;
     0.000000;0.000000;,
     1.000000;0.000000;,
     0.000000;-1.000000;,
     1.000000;-1.000000;,
     0.000000;0.000000;,
     0.000000;-1.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.031848;-0.030000;,
     0.031848;-0.970000;,
     0.964212;-0.970000;,
     0.964212;-0.030000;,
     1.000000;0.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     0.000000;-1.000000;,
     0.000000;0.000000;,
     1.000000;-1.000000;,
     1.000000;0.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;0.000000;,
     0.000000;-1.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.000000;0.000000;,
     0.000000;-1.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.822274;0.000000;,
     0.822274;-1.000000;,
     0.000000;0.000000;,
     0.000000;-1.000000;,
     1.000000;-0.177726;,
     0.000000;-0.177726;,
     1.000000;-1.000000;,
     0.000000;-1.000000;,
     1.000000;0.000000;,
     1.000000;-1.000000;,
     0.177726;0.000000;,
     0.177726;-1.000000;,
     1.000000;-0.177726;,
     0.000000;-0.177726;,
     1.000000;-1.000000;,
     0.000000;-1.000000;;
    }

    VertexDuplicationIndices {
     48;
     16;
     0,
     1,
     2,
     3,
     4,
     5,
     6,
     7,
     8,
     9,
     10,
     11,
     12,
     13,
     14,
     15,
     1,
     0,
     3,
     1,
     7,
     5,
     2,
     3,
     6,
     7,
     0,
     2,
     4,
     6,
     4,
     5,
     8,
     11,
     12,
     14,
     9,
     8,
     13,
     12,
     13,
     15,
     9,
     10,
     11,
     10,
     14,
     15;
    }

    MeshMaterialList {
     1;
     28;
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
     0;

     Material {
      1.000000;1.000000;1.000000;1.000000;;
      0.000000;
      1.000000;1.000000;1.000000;;
      0.000000;0.000000;0.000000;;

      TextureFilename {
       "media\\curtaintopc.jpg";
      }
     }
    }
   }
  }
 }
}

AnimationSet {
 

 Animation {
  

  AnimationKey {
   4;
   2;
   0;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;,
   16160;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;;
  }
  { Scene_Root }
 }

 Animation {
  

  AnimationKey {
   4;
   2;
   0;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,4.412319,-1.800583,0.000000,1.000000;;,
   16160;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,4.412319,-1.800583,0.000000,1.000000;;;
  }
  { Box01 }
 }

 Animation {
  

  AnimationKey {
   4;
   2;
   0;16;1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,4.339180,36.106716,30.114172,1.000000;;,
   16160;16;1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,4.339180,36.106716,30.114172,1.000000;;;
  }
  { Box02 }
 }

 Animation {
  

  AnimationKey {
   4;
   2;
   0;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-51.356995,-40.750980,0.000000,1.000000;;,
   16160;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-51.356995,-40.750980,0.000000,1.000000;;;
  }
  { Box03 }
 }

 Animation {
  

  AnimationKey {
   4;
   2;
   0;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-67.774796,-3.303154,0.000000,1.000000;;,
   16160;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-67.774796,-3.303154,0.000000,1.000000;;;
  }
  { Box04 }
 }

 Animation {
  

  AnimationKey {
   4;
   2;
   0;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,77.762573,-3.046958,0.000000,1.000000;;,
   16160;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,77.762573,-3.046958,0.000000,1.000000;;;
  }
  { Box05 }
 }

 Animation {
  

  AnimationKey {
   4;
   2;
   0;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,60.985413,-40.750980,0.000000,1.000000;;,
   16160;16;1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,60.985413,-40.750980,0.000000,1.000000;;;
  }
  { Box06 }
 }

 Animation {
  

  AnimationKey {
   4;
   2;
   0;16;1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,4.589211,-47.886486,70.472237,1.000000;;,
   16160;16;1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,4.589211,-47.886486,70.472237,1.000000;;;
  }
  { Box07 }
 }
}