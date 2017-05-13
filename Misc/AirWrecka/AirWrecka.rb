#!/usr/bin/env ruby
require 'rmodbus'

class FanBus
  attr_accessor :speed_fh, :speed, :rtu
  
  def initialize(speed_file='C:\Program Files (x86)\rFactor2\SpeedDump.txt')
    puts "Opening #{speed_file}"
    @speed_fh = File.open(speed_file)
    @speed    = 0
    @oldspeed = 0
    puts "Seeking.."
    speed_fh.seek(0, IO::SEEK_END)
    
    puts "Connecting to PLC"
    @rtu = ModBus::RTUClient.connect('COM1', 38400, :parity       => SerialPort::ODD,
                                                     :stop_bits    => 1,
                                                     :data_bits    => 8)
    @rtu.read_retry_timeout = 0.03
    puts "Connected: #{@rtu.inspect}"
    puts "Getting slave, uid1"
    @slave = @rtu.with_slave(1)
    puts "Returned: #{@slave.inspect}"
    puts "PLC Connected, AirWrecka Started"
  end
    
  def tick(newspeed)
    @speed = (newspeed * 2.23).floor
    print "[#{Time.now}] #{speed}mph"
    @slave.holding_registers[29] = speed
    puts " ..set"
  end
  
  def run(interval=0.1)
    loop do
      sleep interval
	  begin
	  speed_fh.seek(-8, IO::SEEK_END)
      update = speed_fh.gets('\n').split("\n").last
	  rescue
	  update = nil
	  end
      unless update.nil?
        tick(update.to_i) if update.match(/\d+/)
      end
    end
  end
end

puts "AirWrecka 0.0.2pre starting"

fan = FanBus.new
fan.run