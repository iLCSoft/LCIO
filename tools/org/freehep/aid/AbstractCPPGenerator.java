// Copyright 2002, SLAC, Stanford University, U.S.A.
package org.freehep.aid;

import java.io.*;
import java.util.*;

import org.freehep.jaco.rtti.*;

/**
 * @author Mark Donszelmann
 * @version $Id: AbstractCPPGenerator.java,v 1.1 2003-05-07 09:38:18 gaede Exp $
 */
public abstract class AbstractCPPGenerator extends AbstractGenerator {

    protected final static String language = "cpp";

    protected Properties typeProperties;
    protected Properties sysIncludeProperties;
    protected Properties includeProperties;
    protected Properties valueProperties;
    protected CPPTypeConverter converter;

    public AbstractCPPGenerator(String propDir) {
        super();

        properties.setProperty("cpp", "true");
        sysIncludeProperties = AidUtil.loadProperties(getClass(), propDir, "aid.sysincludes."+language+".properties");
        includeProperties = AidUtil.loadProperties(getClass(), propDir, "aid.includes."+language+".properties");
        valueProperties   = AidUtil.loadProperties(getClass(), propDir, "aid.values."+language+".properties");
        typeProperties    = AidUtil.loadProperties(getClass(), propDir, "aid.types."+language+".properties");
        converter = new CPPTypeConverter(propDir);
    }

    protected abstract String prefix();

    protected String namespace(IClass clazz) {
        return converter.namespace(clazz.getPackageName());
    }

    protected SortedSet printIncludeStatements(IClass clazz) {
        SortedSet sysIncludes = new TreeSet();
        SortedSet includes = new TreeSet();
        SortedSet types = new TreeSet();

        includeStatements(clazz, sysIncludes, includes, namespace(clazz), types);

        Iterator it = sysIncludes.iterator();
        if (it.hasNext()) out.println();
        while (it.hasNext()) {
            String includeName = (String)it.next();
            if (includeName.indexOf("::") >= 0) System.err.println("WARNING: "+clazz.getName()+" does not map '"+includeName+"' to a proper include file...");
            out.println("#include <"+includeName+">");
        }

        it = includes.iterator();
        if (it.hasNext()) out.println();
        while (it.hasNext()) {
            String includeName = (String)it.next();
            if (includeName.indexOf("::") >= 0) System.err.println("WARNING: "+clazz.getName()+" does not map '"+includeName+"' to a proper include file...");
            out.println("#include \""+includeName+"\"");
        }

        return types;
    }

    protected void includeStatements(IClass clazz, SortedSet sysIncludes, SortedSet includes, String namespace, SortedSet types) {

        IMethod[] methods = clazz.getMethods();
        for (int m=0; m<methods.length; m++) {
            // check return type
            includeFrom(methods[m].getReturnType(), prefix()+clazz.getName(), sysIncludes, includes, namespace, types);

            INamedType[] parameterTypes = methods[m].getParameterTypes();
            for (int p=0; p<parameterTypes.length; p++) {
                includeFrom(parameterTypes[p].getType(), prefix()+clazz.getName(), sysIncludes, includes, namespace, types);
                // FIXME: need to do something with init values used from different class
            }
	    //fg--
	    // add includes for exceptions
	    String[] exceptionTypes = methods[m].getExceptionTypes();
	    for (int e=0; e<exceptionTypes.length; e++) {
		includeFrom(exceptionTypes[e], prefix()+clazz.getName(), sysIncludes, includes, namespace, types);
	    }
	    //fg--
        }
    }

//    protected void includeFrom(IType type, String className, SortedSet sysIncludes, SortedSet includes, SortedSet types) {
//        includeFrom(type, className, sysIncludes, includes, "", types);
//   }

    protected void includeFrom(IType type, String className, SortedSet sysIncludes, SortedSet includes, String namespace, SortedSet types) {
        if (type.getDimension() > 0) sysIncludes.add("vector");
        if (type.isPrimitive()) return;
        IType[] subTypes = type.getTypes();
        for (int i=0; i<subTypes.length; i++) {
            includeFrom(subTypes[i], className, sysIncludes, includes, namespace, types);
        }
        if (type.getName().equals("[]")) return;
        includeFrom(type.getName(), className, sysIncludes, includes, namespace, types);
    }

    protected void includeFrom(String name, String className, SortedSet sysIncludes, SortedSet includes, String namespace, SortedSet types) {
        if (name == null) return;

        name = converter.qualifiedName(name, namespace);

        String inc = sysIncludeProperties.getProperty(name, null);
        if (inc != null) {
            sysIncludes.add(inc);
            return;
        }

        inc = includeProperties.getProperty(name, null);
        if (inc != null) {
            includes.add(inc);
            return;
        }

        if (!name.equals("") && !name.equals(className)) {
            types.add(name);
        }
    }

    protected String namedType(INamedType namedType, String nameSpace) {
        StringBuffer s = new StringBuffer();
        s.append(converter.type(namedType.getType(), nameSpace));
        s.append(" ");
        s.append(namedType.getName());
        return s.toString();
    }
}

