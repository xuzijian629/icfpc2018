`ulimit -s $((2**15))`
files = `ls mdl`.split
type = gets.chomp
unless ['fa', 'fd', 'fr'].include? type
  exit
end

files = files.select{|i| i[type.upcase]}
if type == 'fr'
  for file in files
    if file[6..8] == 'src'
      next
    end
    `echo "mdl/#{file}\nmdl/#{file[0..5]}tgt.mdl\nsubmission/#{file[0..4]}.nbt" | ./main`
    stat = `echo $?`.to_i
    if stat != 0
      puts "Error at mdl/#{file}"
      exit 1
    end
  end
  exit
end

for file in files
  `echo "mdl/#{file}\nsubmission/#{file[0..4]}.nbt" | ./main`
  stat = `echo $?`.to_i
  if stat != 0
    puts "Error at mdl/#{file}"
    exit 1
  end
end
