/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef VIZKIT3D_WORLD_TASK_TASK_HPP
#define VIZKIT3D_WORLD_TASK_TASK_HPP

#include "vizkit3d_world/TaskBase.hpp"
#include <vizkit3d_world/Vizkit3dWorld.hpp>
#include <map>

namespace vizkit3d_world {

    /*! \class Task
     * \brief The task context provides and requires services. It uses an ExecutionEngine to perform its functions.
     * Essential interfaces are operations, data flow ports and properties. These interfaces have been defined using the oroGen specification.
     * In order to modify the interfaces you should (re)use oroGen and rely on the associated workflow.
     *
     * \details
     * The name of a TaskContext is primarily defined via:
     \verbatim
     deployment 'deployment_name'
         task('custom_task_name','vizkit3d_world::Task')
     end
     \endverbatim
     *  It can be dynamically adapted when the deployment is called with a prefix argument.
     */
    class Task : public TaskBase
    {
	friend class TaskBase;

    private:

        /**
         * set the posfix
         */
        static const std::string JOINTS_CMD_POSFIX;


        /**
         * map the model name and the port name
         */
        std::map<std::string, std::string> mapModel;

        /**
         * map the port name and the port object
         */
        std::map<std::string, RTT::base::PortInterface*> mapPorts;

        bool showGui;

    protected:

        /**
         * Used to manage the scene in C/C++
         */
	    Vizkit3dWorld *vizkit3dWorld;

    public:
        /** TaskContext constructor for Task
         * \param name Name of the task. This name needs to be unique to make it identifiable via nameservices.
         * \param initial_state The initial TaskState of the TaskContext. Default is Stopped state.
         */
        Task(std::string const& name = "vizkit3d_world::Task");

        /** TaskContext constructor for Task
         * \param name Name of the task. This name needs to be unique to make it identifiable for nameservices.
         * \param engine The RTT Execution engine to be used for this task, which serialises the execution of all commands, programs, state machines and incoming events for a task.
         *
         */
        Task(std::string const& name, RTT::ExecutionEngine* engine);

        /** Default deconstructor of Task
         */
	~Task();

        /** This hook is called by Orocos when the state machine transitions
         * from PreOperational to Stopped. If it returns false, then the
         * component will stay in PreOperational. Otherwise, it goes into
         * Stopped.
         *
         * It is meaningful only if the #needs_configuration has been specified
         * in the task context definition with (for example):
         \verbatim
         task_context "TaskName" do
           needs_configuration
           ...
         end
         \endverbatim
         */
        bool configureHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to Running. If it returns false, then the component will
         * stay in Stopped. Otherwise, it goes into Running and updateHook()
         * will be called.
         */
        bool startHook();

        /** This hook is called by Orocos when the component is in the Running
         * state, at each activity step. Here, the activity gives the "ticks"
         * when the hook should be called.
         *
         * The error(), exception() and fatal() calls, when called in this hook,
         * allow to get into the associated RunTimeError, Exception and
         * FatalError states.
         *
         * In the first case, updateHook() is still called, and recover() allows
         * you to go back into the Running state.  In the second case, the
         * errorHook() will be called instead of updateHook(). In Exception, the
         * component is stopped and recover() needs to be called before starting
         * it again. Finally, FatalError cannot be recovered.
         */
        void updateHook();

        /** This hook is called by Orocos when the component is in the
         * RunTimeError state, at each activity step. See the discussion in
         * updateHook() about triggering options.
         *
         * Call recover() to go back in the Runtime state.
         */
        void errorHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Running to Stopped after stop() has been called.
         */
        void stopHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to PreOperational, requiring the call to configureHook()
         * before calling start() again.
         */
        void cleanupHook();


        /**
         * Create and setup dynamic input ports
         * Each model has an input port to set the joints positions and this
         * port receives a list of base::samples::JointState
         *
         */
        void setupJointsPorts();

        /**
         * Release joints input ports
         */
        void releaseJointsPorts();


        /**
         * Update joint informations
         * read information from input ports and pass to vizkit3d world
         */
        void updateJoints();

        /**
         * Update model pose
         */
        void updatePose();
    };
}

#endif
