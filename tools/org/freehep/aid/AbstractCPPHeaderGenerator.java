// Copyright 2002, SLAC, Stanford University, U.S.A.
package org.freehep.aid;

import java.io.*;
import java.util.*;

import org.freehep.jaco.rtti.*;

/**
 * @author Mark Donszelmann
 * @version $Id: AbstractCPPHeaderGenerator.java,v 1.2 2003-06-10 10:02:07 gaede Exp $
 */
public abstract class AbstractCPPHeaderGenerator extends AbstractCPPGenerator {

    public AbstractCPPHeaderGenerator(String propDir) {
        super(propDir);

        properties.setProperty("cpp.header", "true");
    }

    public String directory(IClass clazz) {
        return "";
    }

    protected String prefix() {
        return "";
    }

    protected abstract void printClassHeader(IClass clazz);
    protected void printPrivate(IClass clazz) {
    }
    protected void printProtected(IClass clazz) {
    }
    protected void printPublic(IClass clazz) {
    }

    public void print(PrintWriter out, IClass clazz) {
        this.out = out;

        out.println("// -*- C++ -*-");

        warning();

        String namespace = namespace(clazz);
        if (!namespace.equals("")) {
            namespace = namespace.replaceAll("::","_")+"_";
        }
        String define = prefix()+namespace+prefix()+clazz.getName()+"_H";
        define = define.toUpperCase();
        out.println("#ifndef "+define);
        out.println("#define "+define+" 1");

        String[] packageComments = clazz.getPackageComments(language);
        if (packageComments.length > 0) out.println();
        for (int i=0; i<packageComments.length; i++) {
            out.print(packageComments[i]);
        }

        SortedSet types = printIncludeStatements(clazz);

        // print specific includes which are in the comments
        String[] classComments = clazz.getComments(language);
        for (int i=0; i<classComments.length; i++) {
            if (classComments[i].indexOf("#include") >= 0) {
                out.print(classComments[i]);
            }
        }

        boolean writeClass = printDefines(clazz);

        if (writeClass) {
            String[] namespaces = (prefix()+namespace(clazz)).split("::");
            for (int i=0; i<namespaces.length; i++) {
                if (i == 0) out.println();
                out.println("namespace "+namespaces[i]+" {");
            }

            Iterator it = types.iterator();
            if (it.hasNext()) out.println();
            while (it.hasNext()) {
                out.println("class "+it.next()+";");
            }

            out.println();
            String[] comments = clazz.getComments(language);
            for (int i=0; i<comments.length; i++) {
                if (comments[i].indexOf("#include") < 0) {
                    out.print(comments[i]);
                }
            }
            printClassHeader(clazz);

            printPrivate(clazz);

            printProtected(clazz);

            printPublic(clazz);

            IConstructor[] constructors = clazz.getConstructors();
            for (int i=0; i<constructors.length; i++) {
                printConstructor(constructors[i], namespace(clazz));
            }

            IMethod[] methods = clazz.getMethods();
            for (int i=0; i<methods.length; i++) {
                printMethod(methods[i], namespace(clazz));
            }

            IField[] fields = clazz.getFields();
            for (int i=0; i<fields.length; i++) {
                printField(fields[i], namespace(clazz));
            }

            String[] eocComments = clazz.getEOCComments(language);
            if (eocComments.length>0) out.println();
            for (int i=0; i<eocComments.length; i++) {
                out.print(eocComments[i]);
            }

            out.println("}; // class");

            for (int i=namespaces.length-1; i>=0; i--) {
                out.println("}; // namespace "+namespaces[i]);
            }
        }

        String[] eofComments = clazz.getEOFComments(language);
        if (eofComments.length > 0) out.println();
        for (int i=0; i<eofComments.length; i++) {
            out.print(eofComments[i]);
        }
        out.println("#endif /* ifndef "+define+" */");

	//        return false;
    }

    protected void printConstructor(IConstructor constructor, String nameSpace) {
    }

    protected void printMethod(IMethod method, String nameSpace) {

	//fg--
	// check property to see wether we use exceptions in cpp - old default: false
	boolean useExceptions = false ;
	if ( typeProperties.getProperty( "useExceptions" ,"false" ).equalsIgnoreCase("true")  )
	    useExceptions = true ;
	//fg--

        String[] exceptionTypes = method.getExceptionTypes();

        out.println();
        String[] comments = method.getComments(language);
        boolean returnWritten = false;
        for (int i=0; i<comments.length; i++) {
            // remove throws clause and replace by return
            int index = comments[i].indexOf("@throws");
            if (index >= 0 && !useExceptions ) {   //fg---
                if (!returnWritten ) {
                    if ((exceptionTypes.length > 0) && (method.getReturnType().isVoid())) {
                        int space = index+7;
                        while (Character.isWhitespace(comments[i].charAt(space))) space++;
                        while (!Character.isWhitespace(comments[i].charAt(space))) space++;
                        String value = valueProperties.getProperty("returnInCaseOfException", "true");
                        out.print(comments[i].substring(0, index)+"@return "+value+comments[i].substring(space));
                        returnWritten = true;
                    }
                }
            } else {
                out.print(comments[i]);
            }
        }

        INamedType[] parameterTypes = method.getParameterTypes();

	out.print("    virtual ");

        if ((exceptionTypes.length > 0) && (method.getReturnType().isVoid()) && !useExceptions ) { //fg--
            out.print("bool");
        } else {
            out.print(converter.type(method.getReturnType(), nameSpace));
        }
        out.print(" ");
        out.print(method.getName());
        out.print("(");

        for (int i=0; i<parameterTypes.length; i++)
        {
            out.print(namedType(parameterTypes[i], nameSpace));
            if (i<parameterTypes.length-1) out.print(", ");
        }
        out.print(")");
        if (method.isConst()) {
            out.print(" const");
        }

	//fg--
	// add throw statement if needed
	if( (exceptionTypes.length > 0) && useExceptions ){
	    out.print(" throw ("); 
            for (int e=0; e<exceptionTypes.length; e++) {
		if( e != 0 ) out.print( ", " ) ;
		out.print( converter.qualifiedName( exceptionTypes[e], nameSpace)  ) ;		
            }
	    out.print(") "); 
	}
	//fg--

    }

    protected abstract boolean printDefines(IClass clazz);
    protected abstract void printField(IField field, String nameSpace);
}

