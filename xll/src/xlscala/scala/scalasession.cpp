#include <xlscala/scala/scalasession.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <xlscala/xl/xl.hpp>
#include <xlscala/xl/xlsession.hpp>



namespace xlscala {

	/******************************************************************************************
		ScalaSession
	******************************************************************************************/
	ScalaSession::ScalaSession()
	: _cout(XLSCALA_MAXLINES), _thrd(&send) {
            _index = 0;
            _close = false;
	}

	ScalaSession::~ScalaSession() {
	}
}
