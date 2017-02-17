/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 * Copyright 2017 Red Hat, Inc.
 */

#include "nm-default.h"

#include "nm-setting-metadata.h"

#include "nm-setting-connection.h"
#include "nm-setting-wired.h"

/*****************************************************************************/

struct NMMetaType {
};

typedef struct {
	struct NMMetaType parent;
} NMMetaTypeGroup;

typedef struct {
	NMMetaTypeGroup parent;
} NMMetaTypeGroupConnection;

struct NMMetaData {
	NMMetaType *type;
};

typedef enum {
	META_DATA_CONNECTION_ANY,
	META_DATA_CONNECTION_UNKNOWN,

	META_DATA_CONNECTION_BLUETOOTH,
	META_DATA_CONNECTION_BOND,
	META_DATA_CONNECTION_BRIDGE,
	META_DATA_CONNECTION_GSM,
	META_DATA_CONNECTION_INFINIBAND,
	META_DATA_CONNECTION_TEAM,
	META_DATA_CONNECTION_VLAN,
	META_DATA_CONNECTION_VPN,
	META_DATA_CONNECTION_WIRED,
	META_DATA_CONNECTION_WIRELESS,

	META_DATA_NUM,
} MetaDataConnectionType;


#define _META_DATA_GET_DEFINE(type) \
static NMMetaData * \
_meta_data_get_##type (void) \
{ \
	g_return_val_if_reached (NULL); \
}

_META_DATA_GET_DEFINE (META_DATA_CONNECTION_ANY);
_META_DATA_GET_DEFINE (META_DATA_CONNECTION_WIRED);

static NMMetaData *(*const meta_data_get_fcn[META_DATA_NUM]) (void) = {
	_meta_data_get_META_DATA_CONNECTION_ANY,
	_meta_data_get_META_DATA_CONNECTION_WIRED,
};

static NMMetaData *
meta_data_get (MetaDataConnectionType type)
{
	nm_assert (type < META_DATA_NUM && (int) META_DATA_NUM > 0);
	nm_assert (meta_data_get_fcn[type]);

	return meta_data_get_fcn[type] ();
}

NMMetaData *nm_meta_data_get_for_connection (NMConnection *connection)
{
	const char *type;

	g_return_val_if_fail (NM_IS_CONNECTION (connection), NULL);

	type = nm_connection_get_connection_type (connection);
	if (!type)
		return meta_data_get (META_DATA_CONNECTION_ANY);
	else if (nm_streq (type, NM_SETTING_WIRED_SETTING_NAME))
		return meta_data_get (META_DATA_CONNECTION_WIRED);
	else if (nm_streq (type, NM_SETTING_BLUETOOTH_SETTING_NAME))
		return meta_data_get (META_DATA_CONNECTION_BLUETOOTH);
	else if (nm_streq (type, NM_SETTING_BOND_SETTING_NAME))
		return meta_data_get (META_DATA_CONNECTION_BOND);
	else if (nm_streq (type, NM_SETTING_BRIDGE_SETTING_NAME))
		return meta_data_get (META_DATA_CONNECTION_BRIDGE);
	else if (nm_streq (type, NM_SETTING_GSM_SETTING_NAME))
		return meta_data_get (META_DATA_CONNECTION_GSM);
	else if (nm_streq (type, NM_SETTING_INFINIBAND_SETTING_NAME)) 		return meta_data_get (META_DATA_CONNECTION_INFINIBAND);
	else if (nm_streq (type, NM_SETTING_TEAM_SETTING_NAME)) 		return meta_data_get (META_DATA_CONNECTION_TEAM);
	else if (nm_streq (type, NM_SETTING_VLAN_SETTING_NAME)) 		return meta_data_get (META_DATA_CONNECTION_VLAN);
	else if (nm_streq (type, NM_SETTING_VPN_SETTING_NAME)) 		return meta_data_get (META_DATA_CONNECTION_VPN);
	else if (nm_streq (type, NM_SETTING_WIRED_SETTING_NAME)) 		return meta_data_get (META_DATA_CONNECTION_WIRED);
	else if (nm_streq (type, NM_SETTING_WIRELESS_SETTING_NAME)) 		return meta_data_get (META_DATA_CONNECTION_WIRELESS);

	return meta_data_get (META_DATA_CONNECTION_ANY);
}

/*****************************************************************************/

const NMSetting8021xSchemeVtable nm_setting_8021x_scheme_vtable[] = {
	[NM_SETTING_802_1X_SCHEME_TYPE_CA_CERT] = {
		.setting_key            = NM_SETTING_802_1X_CA_CERT,
		.scheme_func            = nm_setting_802_1x_get_ca_cert_scheme,
		.format_func            = NULL,
		.path_func              = nm_setting_802_1x_get_ca_cert_path,
		.blob_func              = nm_setting_802_1x_get_ca_cert_blob,
		.uri_func               = nm_setting_802_1x_get_ca_cert_uri,
		.passwd_func            = nm_setting_802_1x_get_ca_cert_password,
		.pwflag_func            = nm_setting_802_1x_get_ca_cert_password_flags,
		.file_suffix            = "ca-cert",
	},

	[NM_SETTING_802_1X_SCHEME_TYPE_PHASE2_CA_CERT] = {
		.setting_key            = NM_SETTING_802_1X_PHASE2_CA_CERT,
		.scheme_func            = nm_setting_802_1x_get_phase2_ca_cert_scheme,
		.format_func            = NULL,
		.path_func              = nm_setting_802_1x_get_phase2_ca_cert_path,
		.blob_func              = nm_setting_802_1x_get_phase2_ca_cert_blob,
		.uri_func               = nm_setting_802_1x_get_phase2_ca_cert_uri,
		.passwd_func            = nm_setting_802_1x_get_phase2_ca_cert_password,
		.pwflag_func            = nm_setting_802_1x_get_phase2_ca_cert_password_flags,
		.file_suffix            = "inner-ca-cert",
	},

	[NM_SETTING_802_1X_SCHEME_TYPE_CLIENT_CERT] = {
		.setting_key            = NM_SETTING_802_1X_CLIENT_CERT,
		.scheme_func            = nm_setting_802_1x_get_client_cert_scheme,
		.format_func            = NULL,
		.path_func              = nm_setting_802_1x_get_client_cert_path,
		.blob_func              = nm_setting_802_1x_get_client_cert_blob,
		.uri_func               = nm_setting_802_1x_get_client_cert_uri,
		.passwd_func            = nm_setting_802_1x_get_client_cert_password,
		.pwflag_func            = nm_setting_802_1x_get_client_cert_password_flags,
		.file_suffix            = "client-cert",
	},

	[NM_SETTING_802_1X_SCHEME_TYPE_PHASE2_CLIENT_CERT] = {
		.setting_key            = NM_SETTING_802_1X_PHASE2_CLIENT_CERT,
		.scheme_func            = nm_setting_802_1x_get_phase2_client_cert_scheme,
		.format_func            = NULL,
		.path_func              = nm_setting_802_1x_get_phase2_client_cert_path,
		.blob_func              = nm_setting_802_1x_get_phase2_client_cert_blob,
		.uri_func               = nm_setting_802_1x_get_phase2_client_cert_uri,
		.passwd_func            = nm_setting_802_1x_get_phase2_client_cert_password,
		.pwflag_func            = nm_setting_802_1x_get_phase2_client_cert_password_flags,
		.file_suffix            = "inner-client-cert",
	},

	[NM_SETTING_802_1X_SCHEME_TYPE_PRIVATE_KEY] = {
		.setting_key            = NM_SETTING_802_1X_PRIVATE_KEY,
		.scheme_func            = nm_setting_802_1x_get_private_key_scheme,
		.format_func            = nm_setting_802_1x_get_private_key_format,
		.path_func              = nm_setting_802_1x_get_private_key_path,
		.blob_func              = nm_setting_802_1x_get_private_key_blob,
		.uri_func               = nm_setting_802_1x_get_private_key_uri,
		.passwd_func            = nm_setting_802_1x_get_private_key_password,
		.pwflag_func            = nm_setting_802_1x_get_private_key_password_flags,
		.file_suffix            = "private-key",
	},

	[NM_SETTING_802_1X_SCHEME_TYPE_PHASE2_PRIVATE_KEY] = {
		.setting_key            = NM_SETTING_802_1X_PHASE2_PRIVATE_KEY,
		.scheme_func            = nm_setting_802_1x_get_phase2_private_key_scheme,
		.format_func            = nm_setting_802_1x_get_phase2_private_key_format,
		.path_func              = nm_setting_802_1x_get_phase2_private_key_path,
		.blob_func              = nm_setting_802_1x_get_phase2_private_key_blob,
		.uri_func               = nm_setting_802_1x_get_phase2_private_key_uri,
		.passwd_func            = nm_setting_802_1x_get_phase2_private_key_password,
		.pwflag_func            = nm_setting_802_1x_get_phase2_private_key_password_flags,
		.file_suffix            = "inner-private-key",
	},

	[NM_SETTING_802_1X_SCHEME_TYPE_UNKNOWN] = { NULL },
};

/*****************************************************************************/
