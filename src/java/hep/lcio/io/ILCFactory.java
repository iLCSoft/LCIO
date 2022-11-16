// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================

package hep.lcio.io;


/**Factory to create LCWriter and Reader object for the known data 
 * formats, e.g. SIO. The implementation (LCFactory) will be a singleton class 
 * that knows all concrete reader/writer implementations.
 * 
 * @author gaede
 * @version Mar 7, 2003
 */
public interface ILCFactory {

//     /** Returns the instance of the factory.
//      * In the future we need to specify the data format here...
//      * This doesn't work for java as an interface can't have static members ...
//      */
    // this needs more thought ....
//           
//     public  LCFactory getInstance() ;
//       
/**Creates an LCWriter object for the current persistency type.
 */
    public LCWriter createLCWriter();

/**Creates an LCReader object for the current persistency type.
 */
    public LCReader createLCReader();
/**Creates an LCReader object for the current persistency type.
 */
    public LCReader createLCReader(int readerFlag);
} // class or interface
