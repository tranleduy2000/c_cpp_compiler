package com.duy.common.purchase;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.anjlab.android.iab.v3.BillingProcessor;
import com.anjlab.android.iab.v3.TransactionDetails;

public class InAppPurchaseActivity extends AppCompatActivity implements BillingProcessor.IBillingHandler {
    private BillingProcessor mBillingProcessor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBillingProcessor = new BillingProcessor(this, Premium.BASE64_KEY, this);
    }

    protected void showDialogPurchase() {

    }

    /*
     * Called when BillingProcessor was initialized and it's ready to purchase
     */
    @Override
    public void onBillingInitialized() {

    }

    /*
     * Called when requested PRODUCT ID was successfully purchased
     */
    @Override
    public void onProductPurchased(String productId, TransactionDetails details) {

    }

    /*
     * Called when some error occurred. See Constants class for more details
     *
     * Note - this includes handling the case where the user canceled the buy dialog:
     * errorCode = Constants.BILLING_RESPONSE_RESULT_USER_CANCELED
     */
    @Override
    public void onBillingError(int errorCode, Throwable error) {

    }

    /*
     * Called when purchase history was restored and the list of all owned PRODUCT ID's
     * was loaded from Google Play
     */
    @Override
    public void onPurchaseHistoryRestored() {

    }
}
