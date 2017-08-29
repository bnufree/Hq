// -------------------------------------------------------------------------------
//  Project:		Qt Webbased File Handling
//
//  File:           debugout.h
//
// (c) 2012 - 2013 by Oblivion Software/Norbert Schlia
// This source code is licensed under The Code Project Open License (CPOL),
// see http://www.codeproject.com/info/cpol10.aspx
// -------------------------------------------------------------------------------
//
#pragma once

#ifndef DEBUGOUT_H
#define DEBUGOUT_H

#include <QDebug>
#include <QTime>

#define QDEBUGTS    qDebug() << qPrintable(QTime::currentTime().toString("hh:mm:ss.zzz") + " " + QString(__PRETTY_FUNCTION__) + ":")

#endif // DEBUGOUT_H
