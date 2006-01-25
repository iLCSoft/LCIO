#!/bin/sh

. setup.sh

ant aid.generate     
ant cpp             
ant aid              
ant f77              
