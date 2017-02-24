#include "dataexchange.h"

DataExchange* DataExchange::mInstance = NULL;
DataExchange::DataExchange(QObject *parent) : QObject(parent)
{

}

DataExchange::~DataExchange()
{
    if(mInstance != NULL)
    {
        delete mInstance;
        mInstance = NULL;
    }

}

DataExchange* DataExchange::getInstance()
{
    if(mInstance == NULL)
    {
        mInstance = new DataExchange;
    }

    return mInstance;
}



