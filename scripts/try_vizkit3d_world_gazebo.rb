#!/usr/bin/ruby
# In this sample test we send an array with the model state
# updateHook read this information and update the model joints and positions
#
require 'orocos'
require 'orocos/async'
require 'sdf'
require 'readline'
require 'Qt4'
require 'io/console'

include Orocos
Orocos.initialize

# connect port between gazebo task and the models in the world task
# the vikiz3d_world task load a sdf which define the scene
# the scene there are several models
# the ports between the models in the vizkit3d_world and gazebo are connected by model name
#
# @param [SDF::Root] sdf the sdf representation
# @param [Orocos::TaskContext] world_task the vizkit3d_world orogen task
def resolve_gazebo_tasks(sdf, world_task)
    sdf.each_model(recursive: true) do |model|
        gazebo_task_name = "gazebo:#{model.full_name.gsub('::',  ':')}"

        joints_cmd = world_task.port("#{model.name}:joints_cmd")
        pose_cmd = world_task.pose_cmd
		puts "#{model.name}:joints_cmd"
        connect_port_to gazebo_task_name, "joints_samples", joints_cmd
        connect_port_to gazebo_task_name, "pose_samples", pose_cmd
    end
    joints_cmd = world_task.port("lolo_auv:joints_cmd")
    pose_cmd = world_task.pose_cmd
    connect_port_to "gazebo:empty_underwater:lolo_auv", "joints_samples", joints_cmd
    connect_port_to "gazebo:empty_underwater:lolo_auv", "pose_samples", pose_cmd
end

# connect port in the asynchronous proxy
#
# @param [String] task_name the name of the proxy task that will be connected
# @param [String] port_name the name of the output port task
# @param [Orocos::InputPort] port the input port that will be connected
def connect_port_to(task_name, port_name, port)

    from_port = Orocos::Async.proxy(task_name).port(port_name)

    from_port.on_unreachable do
        $stderr.puts "the port #{port_name} from task #{task_name} it is unreachable"
    end

    #from_port.once_on_reachable do
    from_port.on_reachable do
        begin
            from_port.to_orocos_port.connect_to port
            puts "Managed to connect port #{port_name} from task #{task_name}"
        rescue
            $stderr.puts "error when try to connect port"
        end
    end
end

# read a key without block
#
def readkey
    system('stty raw -echo') # => Raw mode, no echo
    c = (STDIN.read_nonblock(1).ord rescue nil)
    system('stty -raw echo') # => Reset terminal mode
    c
end

# run qt event loop and Async step
#
#def run_async(app)
def run_async
    stop = false

    puts "Press Crtl+C to quit..."
    trap('INT') do
        puts "\nFinish..."
        stop = true
    end

    while !stop
        Orocos::Async.step
        sleep(0.01)
		#app.processEvents()
		#puts "Waiting.."
    end

end


# robot_model is a dependency of this orogen package.
# insert robot_model/test_data folder in the model paths
#model_path = ["#{ENV['AUTOPROJ_CURRENT_ROOT']}/gui/robot_model/test_data"]
model_path = ["/home/nbore/Workspace/ros/uw_ws/src/uuv_simulator/uuv_descriptions/worlds"]

path = File.join('/home/nbore/Workspace/ros/uw_ws/src/uuv_simulator/uuv_descriptions/worlds', 'empty_underwater.world')
SDF::XML.model_path << model_path
sdf = SDF::Root.load(path)

Orocos.run 'vizkit3d_world::Task' => 'vizkit3d_world' do

    #app = Qt::Application.new(ARGV)
    world_task = TaskContext.get 'vizkit3d_world'
	puts world_task

    #set an array with model paths
    world_task.model_paths = model_path
    #path to world file
    world_task.world_file_path = path
    #show gui
    world_task.show_gui = true

    world_task.to_proxy.on_state_change do |state|
        case state
        when :RUNNING
            puts "resolve_gazebo_tasks"
            resolve_gazebo_tasks(sdf, world_task)
        end
    end

    world_task.configure
    world_task.start
    #run_async(app)
    run_async
    world_task.stop

end

