#include "constr_SEQUENCE_OF.h"

/* This function is missing and would lead to a compilation error.
 * See https://github.com/vlm/asn1c/issues/42 */

asn_enc_rval_t
SET_OF_encode_uper(asn_TYPE_descriptor_t *td,
                   asn_per_constraints_t *constraints, void *sptr,
                   asn_per_outp_t *po)
{
    return SEQUENCE_OF_encode_uper(td, constraints, sptr, po);
}
