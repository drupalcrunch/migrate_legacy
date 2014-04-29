<?php
/**
 * @file
 * Migration for player data.
 */

class LegacyUserMigration extends LegacyBaseMigration {

    public function __construct($arguments) {
        parent::__construct($arguments);

        $this->description = t('Import user data from legacy database.');

        $query = $this->getConnection()->select('individual_login', 'il');
        $query->join('individual', 'i', 'il.member_id = i.member_id');
        $query->join('member_contact_details', 'mcd', 'il.member_id = mcd.member_id');
        $query->fields('i', array('family_name', 'other_name', 'knownas', 'dob', 'bio'));
        $query->fields('il', array('member_id', 'username'));
        $query->fields('mcd', array('email_primary', 'phone_primary', 'url'));
        $query->isNotNull('mcd.email_primary');
        $query->isNotNull('il.username');
        $query->condition('il.username', '', '<>');

        $this->source = new MigrateSourceSQL($query);

        $this->destination = new MigrateDestinationUser();

        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'member_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'description' => 'Member ID',
                    'alias' => 'il',
                ),
            ),
            MigrateDestinationUser::getKeySchema()
        );

        $this->addFieldMapping('name', 'username');
        $this->addFieldMapping('mail', 'email_primary');
        $this->addFieldMapping('field_family_name', 'family_name');
        $this->addFieldMapping('field_other_name', 'other_name');
        $this->addFieldMapping('field_known_as', 'knownas');
        $this->addFieldMapping('field_date_of_birth', 'dob');
        $this->addFieldMapping('field_biography', 'bio');
        $this->addFieldMapping('field_biography:format')
            ->defaultValue('full_html');

        $this->addFieldMapping('field_link', 'url');
        $this->addFieldMapping('field_phone_number', 'phone_primary');
        $this->addFieldMapping('roles')
            ->defaultValue(DRUPAL_AUTHENTICATED_RID);

        $this->addUnmigratedDestinations(array(
            'pass', 'created', 'access', 'login', 'role_names', 'picture', 'signature', 'status',
            'signature_format', 'timezone', 'language', 'theme', 'init', 'data', 'is_new'
        ));
    }
}
