#ifndef STATE_H__
#define STATE_H__

#include <vector>
#include <memory>
#include <cassert>

class StateManager; //because things have to be declared before use
class State {
private:
	friend class StateManager; //because friends can access your.... privates... :))
	StateManager *manager;
	bool m_isActive;
protected:
	void popSelf(unsigned int levels) {
		//naive
		manager->pop(levels);
	}
	bool isActive() const {
		return m_isActive;
	}
	bool isOnStack() const {
		return manager != nullptr;
	}
public:
	State() : manager(nullptr), m_isActive(false) {}

	// = 0 means that the subclass must override this method
	virtual void update(double dt) = 0; 
	virtual void draw() = 0;

	//default implementation: do nothing
	virtual void onActivate() {} 
	virtual void onDeactivate() {}
};

class StateManager {
	std::vector<std::shared_ptr<State>> m_states;
public:
	//is this ok?
	//think of what could be called by the state in "onDeactivate" or
	//"onActivate"... will it cause problems?
	//like what if a state pushes another state in its "onActivate"?
	//will it blow up?
	void push(std::shared_ptr<State> &s) {
		assert(s->manager == nullptr && "A state can only be in one manager at a time!");
		std::shared_ptr<State> previous_state;

		if (!m_states.empty()) {
			previous_state = m_states.back();
		}

		s->manager = this;
		s->m_isActive = true;
		m_states.push_back(s);
		s->onActivate();

		if ( previous_state ) {
			previous_state->m_isActive = false;
			previous_state->onDeactivate();
		}
	}
	//this one is actually OK.
	void pop(unsigned int levels = 1) {
		while (levels --> 0 && !m_states.empty()) {
			auto s = m_states.back(); //s gets the type of whatever m_states.back() returns
			m_states.pop_back(); //ensure that the state is not on teh stack when we call "onDeactivate"
			s->manager = nullptr;
			s->m_isActive = false;
			//s is no longer in the stack when we called onDeactivate
			s->onDeactivate(); //this might push states!
		}
		//the one on top might already be activated (especially if something got pushed in onDeactivate)
		if (!m_states.empty() && m_states.back()->m_isActive == false) {
			m_states.back()->m_isActive = true;
			//m_states.back() is at the top of the stack when we called onActivate
			m_states.back()->onActivate();
		}
	}

	void update(double dt) {
		if (m_states.empty()) return; //becase empty case
		m_states.back()->update(dt);
	}

	void draw() {
		if (m_states.empty()) return;
		m_states.back()->draw();
	}


};
#endif