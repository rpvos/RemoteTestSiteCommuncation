foreach ($proto_file in Get-ChildItem ./proto/ ) {
    $proto_file_with_pb = $proto_file.BaseName + ".pb"
    foreach ($header_file in Get-ChildItem ./include/proto) {
        if ($header_file.BaseName -eq $proto_file_with_pb) {
            Remove-Item $header_file
        }
    }
    foreach ($header_file in Get-ChildItem ./src/proto) {
        if ($header_file.BaseName -eq $proto_file_with_pb) {
            Remove-Item $header_file
        }
    }
}

Get-ChildItem ./proto/ | Foreach-Object {
    $name = "proto/" + $_.Name
    python lib/nanopb/generator/nanopb_generator.py $name
    $header = "proto/" + $_.BaseName + ".pb.h"
    $source = "proto/" + $_.BaseName + ".pb.c"
    Move-Item -Path $header -Destination ./include/proto 
    Move-Item -Path $source -Destination ./src/proto 
}


# When using google timestamp
# $file = './include/proto/update.pb.h'
# $find = '#include "google/protobuf/timestamp.pb.h"'
# $replace = '#include "proto/timestamp.pb.h"'
# (Get-Content $file).Replace($find, $replace) | Set-Content $file



