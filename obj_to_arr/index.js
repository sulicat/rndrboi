let fs = require ('fs');
const assimpjs = require ('assimpjs')();

if( process.argv.length != 3 )
{
    console.log("ERROR provide and input file");
    process.exit(-1);
}

var input_file = process.argv[2];

assimpjs.then ((ajs) => {
    // create new file list object
    let fileList = new ajs.FileList ();

    // add model files
    fileList.AddFile (
        input_file,
        fs.readFileSync (input_file)
    );

    // convert file list to assimp json
    let result = ajs.ConvertFileList (fileList, 'assjson');

    // check if the conversion succeeded
    if (!result.IsSuccess () || result.FileCount () == 0) {
        console.log (result.GetErrorCode ());
        return;
    }

    // get the result file, and convert to string
    let resultFile = result.GetFile (0);
    let jsonContent = new TextDecoder ().decode (resultFile.GetContent ());

    // parse the result json
    let resultJson = JSON.parse (jsonContent);

    var verts = resultJson["meshes"][0]["vertices"];
    var normals = resultJson["meshes"][0]["normals"];
    var uv = resultJson["meshes"][0]["texturecoords"][0];
    var faces = resultJson["meshes"][0]["faces"];

    for( var f = 0; f < faces.length; f++ )
    {
        for( var vi = 0; vi < faces[f].length; vi++ )
        {
            var i = faces[f][vi];
            var vx = verts[i*3 + 0];
            var vy = verts[i*3 + 1];
            var vz = verts[i*3 + 2];

            var nx = normals[i*3 + 0];
            var ny = normals[i*3 + 1];
            var nz = normals[i*3 + 2];

            var ux = uv[i*2 + 0] == undefined ? 0 : uv[i*2 + 0];
            var uy = uv[i*2 + 1] == undefined ? 0 : uv[i*2 + 1];

            console.log(vx + ", " + vy + ", " + vz + ", " + nx + ", " + ny + ", " + nz + ", " + ux + ", " + uy + ", ");
        }
    }

});

