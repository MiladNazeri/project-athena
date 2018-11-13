package io.highfidelity.hifiinterface.fragment;

import android.content.SharedPreferences;
import android.media.audiofx.AcousticEchoCanceler;
import android.os.Bundle;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.support.annotation.Nullable;

import io.highfidelity.hifiinterface.R;

public class SettingsFragment extends PreferenceFragment implements SharedPreferences.OnSharedPreferenceChangeListener {

    public native void updateHifiSetting(String group, String key, boolean value);
    public native boolean getHifiSettingBoolean(String group, String key, boolean defaultValue);

    private final String HIFI_SETTINGS_ANDROID_GROUP = "Android";
    private final String HIFI_SETTINGS_AEC_KEY = "aec";
    private final String PREFERENCE_KEY_AEC = "aec";

    private final boolean DEFAULT_AEC_ENABLED = true;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.settings);
        boolean aecAvailable = AcousticEchoCanceler.isAvailable();
        PreferenceManager.setDefaultValues(getContext(), R.xml.settings, false);

        if (!aecAvailable) {
            findPreference(PREFERENCE_KEY_AEC).setEnabled(false);
            updateHifiSetting(HIFI_SETTINGS_ANDROID_GROUP, HIFI_SETTINGS_AEC_KEY, false);
        }

        getPreferenceScreen().getSharedPreferences().edit().putBoolean(PREFERENCE_KEY_AEC,
                aecAvailable && getHifiSettingBoolean(HIFI_SETTINGS_ANDROID_GROUP, HIFI_SETTINGS_AEC_KEY, DEFAULT_AEC_ENABLED)).commit();

    }

    public static SettingsFragment newInstance() {
        SettingsFragment fragment = new SettingsFragment();
        return fragment;
    }

    @Override
    public void onResume() {
        super.onResume();
        getPreferenceScreen().getSharedPreferences().registerOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onPause() {
        super.onPause();
        getPreferenceScreen().getSharedPreferences().unregisterOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
        switch (key) {
            case PREFERENCE_KEY_AEC:
                updateHifiSetting(HIFI_SETTINGS_ANDROID_GROUP, HIFI_SETTINGS_AEC_KEY, sharedPreferences.getBoolean(key, DEFAULT_AEC_ENABLED));
                break;
            default:
                break;
        }
    }
}
