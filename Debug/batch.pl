#! /usr/bin/perl


for ($i=0;$i<100;$i++)
  {
    for ($j=0;$j<100;$j++)
      {
      $x="./URLFilteringOnline /Users/dhay/Desktop/FromShimrit ";
      $y= "var/".$ARGV[0];
      $z=" full_domains";
      $command=$x.$z;
      system($command);
      }
  }
