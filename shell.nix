# shell.nix for C/C++ Environment
{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  name = "C";
  buildInputs = with pkgs; [
    libgcc
    clang-tools

    binutils
    glibc

    man-pages
  ];
  LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
    pkgs.stdenv.cc.cc
  ];
}
