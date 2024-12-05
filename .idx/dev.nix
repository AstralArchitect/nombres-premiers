{pkgs, ...}: {
  channel = "stable-23.11";
  packages = [pkgs.meson pkgs.gnumake pkgs.gcc pkgs.ninja];
}