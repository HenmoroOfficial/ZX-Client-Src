#ifndef __CPPGEN_GNET_SKILL3756
#define __CPPGEN_GNET_SKILL3756
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3756:public Skill
    {
      public:
        enum
        { SKILL_ID = 3756 };
          Skill3756 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3756Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2000;
            }
            void Calculate (Skill * skill) const
            {
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (int (skill->GetPlayer ()->GetSkilldodge () / 9.99));
                  skill->GetPlayer ()->SetVar2 (1);
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3756Stub ():SkillStub (3756)
        {
            occupation = 150;
            name = L"Òµ»ðÉ±II";
            nativename = "Òµ»ðÉ±II";
            icon = "Òµ»ðÉ±II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 3;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 11;
            clearmask = 132;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 9;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            action[0] = "·ÙÏã¹È_»ðÑæ½á½ç";
            action[1] = "·ÙÏã¹È_»ðÑæ½á½ç";
            action[2] = "·ÙÏã¹È_»ðÑæ½á½ç";
            action[3] = "·ÙÏã¹È_»ðÑæ½á½ç";
            action[4] = "·ÙÏã¹È_»ðÑæ½á½ç";
            action[5] = "·ÙÏã¹È_»ðÑæ½á½ç";
            action[6] = "·ÙÏã¹È_»ðÑæ½á½ç";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_»ðÑæ½á½ç";
            action[16] = "·ÙÏã¹È_Æï³Ë_»ðÑæ½á½ç";
            action[17] = "·ÙÏã¹È_Æï³Ë_»ðÑæ½á½ç";
            action[18] = "·ÙÏã¹È_Æï³Ë_»ðÑæ½á½ç";
            action[19] = "·ÙÏã¹È_Æï³Ë_»ðÑæ½á½ç";
            action[20] = "·ÙÏã¹È_Æï³Ë_»ðÑæ½á½ç";
            action[21] = "·ÙÏã¹È_Æï³Ë_»ðÑæ½á½ç";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill3756Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3431 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (800);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000 - 3000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               5 + 2 * skill->GetLevel (),
                               0.5 * skill->GetLevel (), 2 * skill->GetLevel (), 2 * skill->GetLevel (), 2 * skill->GetLevel (), skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () == 1 ? 100 : 0));
            skill->GetVictim ()->SetTime (1000 * (5 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->
                SetRatio ((20 - int (skill->GetCharging () / 200) * 0.1 * skill->GetLevel () - 0.1 * skill->GetPlayer ()->GetVar1 ()) >0 ? (20 -
                                                                                                                                            int
                                                                                                                                            (skill->
                                                                                                                                             GetCharging
                                                                                                                                             () /
                                                                                                                                             200) *
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetLevel
                                                                                                                                            () -
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetPlayer
                                                                                                                                            ()->
                                                                                                                                            GetVar1
                                                                                                                                            ()) : 0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (50760);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () == 1 ? 100 : 0));
            skill->GetVictim ()->SetTime (1000 * (5 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->
                SetRatio ((20 - int (skill->GetCharging () / 200) * 0.1 * skill->GetLevel () - 0.1 * skill->GetPlayer ()->GetVar1 ()) >0 ? (20 -
                                                                                                                                            int
                                                                                                                                            (skill->
                                                                                                                                             GetCharging
                                                                                                                                             () /
                                                                                                                                             200) *
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetLevel
                                                                                                                                            () -
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetPlayer
                                                                                                                                            ()->
                                                                                                                                            GetVar1
                                                                                                                                            ()) : 0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (50760);
            skill->GetVictim ()->SetValue (60);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () == 1 ? 100 : 0));
            skill->GetVictim ()->SetTime (1000 * (5 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->
                SetRatio ((20 - int (skill->GetCharging () / 200) * 0.1 * skill->GetLevel () - 0.1 * skill->GetPlayer ()->GetVar1 ()) >0 ? (20 -
                                                                                                                                            int
                                                                                                                                            (skill->
                                                                                                                                             GetCharging
                                                                                                                                             () /
                                                                                                                                             200) *
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetLevel
                                                                                                                                            () -
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetPlayer
                                                                                                                                            ()->
                                                                                                                                            GetVar1
                                                                                                                                            ()) : 0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (50760);
            skill->GetVictim ()->SetValue (120);
            skill->GetVictim ()->SetCreateobject (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () == 1 ? 100 : 0));
            skill->GetVictim ()->SetTime (1000 * (5 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->
                SetRatio ((20 - int (skill->GetCharging () / 200) * 0.1 * skill->GetLevel () - 0.1 * skill->GetPlayer ()->GetVar1 ()) >0 ? (20 -
                                                                                                                                            int
                                                                                                                                            (skill->
                                                                                                                                             GetCharging
                                                                                                                                             () /
                                                                                                                                             200) *
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetLevel
                                                                                                                                            () -
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetPlayer
                                                                                                                                            ()->
                                                                                                                                            GetVar1
                                                                                                                                            ()) : 0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (50760);
            skill->GetVictim ()->SetValue (180);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () == 1 ? 100 : 0));
            skill->GetVictim ()->SetTime (1000 * (5 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->
                SetRatio ((20 - int (skill->GetCharging () / 200) * 0.1 * skill->GetLevel () - 0.1 * skill->GetPlayer ()->GetVar1 ()) >0 ? (20 -
                                                                                                                                            int
                                                                                                                                            (skill->
                                                                                                                                             GetCharging
                                                                                                                                             () /
                                                                                                                                             200) *
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetLevel
                                                                                                                                            () -
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetPlayer
                                                                                                                                            ()->
                                                                                                                                            GetVar1
                                                                                                                                            ()) : 0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (50760);
            skill->GetVictim ()->SetValue (240);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () == 1 ? 100 : 0));
            skill->GetVictim ()->SetTime (1000 * (5 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->
                SetRatio ((20 - int (skill->GetCharging () / 200) * 0.1 * skill->GetLevel () - 0.1 * skill->GetPlayer ()->GetVar1 ()) >0 ? (20 -
                                                                                                                                            int
                                                                                                                                            (skill->
                                                                                                                                             GetCharging
                                                                                                                                             () /
                                                                                                                                             200) *
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetLevel
                                                                                                                                            () -
                                                                                                                                            0.1 *
                                                                                                                                            skill->
                                                                                                                                            GetPlayer
                                                                                                                                            ()->
                                                                                                                                            GetVar1
                                                                                                                                            ()) : 0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (50760);
            skill->GetVictim ()->SetValue (300);
            skill->GetVictim ()->SetCreateobject (1);
            return true;
        }
#endif
    };
}
#endif
